#!BPY
"""
Name: 'Doom3 MD5'
Blender: 241
Group: 'Export'
Tip: 'Export md5mesh and md5anim files for the Doom3 engine'
"""

# blender2md5.py version 0.94, 2006-02-09
# Exports animated characters and cameras
# from Blender to md5 (Doom3 engine format)
# Copyright (C) 2003,2004,2005,2006 Thomas "der_ton" Hutter, tom-(at)gmx.de,
#                    "der_ton" on www.doom3world.org
# (I also wrote a viewer for md5 files, that is useful
#  for workflow in conjunction with this script,
#  available (as a Win32 binary only) on http://www.doom3world.org)

# Portions of this script are based on:
# blender2cal3D.py version 0.5 by Jean-Baptiste LAMY -- jiba@tuxfamily.org
# (See http://oomadness.tuxfamily.org/en/blender2cal3d)

# HOW TO USE:
# 1 - load the script in Blender's text editor
# 2 - type meta/alt + P
# 3 - specify frame range and filenames in the GUI
#     if you don't fill in one of the filenames when exporting to md5mesh/anim,
#     the corresponding file will not be generated but the other one will
#     (useful when exporting multiple anims for the same md5mesh)


# Questions and comments are welcome!
# If you use this script, a mention that you used this script in your
# production would be nice. :)
# Also, I would love to see what you achieved, please feel free to tell
# me about your results.


# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


# This script is a Blender => MD5Mesh/MD5Anim/MD5Camera exporter

# Grab the latest version of the script here:
# http://home.tiscali.de/der_ton/blender2md5.zip
# http://www.doom3world.org


# Some Doom3world.org subforums where you will find additional infos,
# and where you can ask for support:
# "Blender":
# http://www.doom3world.org/phpbb2/viewforum.php?f=50
# "Modelling and Animations"
# http://www.doom3world.org/phpbb2/viewforum.php?f=3

# Thanks and acknowledgements:
#  - Jiba, for his excellent blender2cal3d exporter, clean and reusable
#  - kat, for his constant testing and feedback
#  - the folks at www.elysiun.org, a forum dedicated to Blender
#  - the many people behind Blender
#  - the many people behind Python


# ADVICES :
# - do not use "Size" keys in Blender's action IPOs

# TODO :
# - Optimization: almost all the time is spent on scene.makeCurrent(), called for
#   updating the IPO curve's values. Updating a single IPO and not the whole scene
#   would speed up a lot.

# - which objects should be output? should be centralized (for mesh-output and bbox output) (maybe let the user select the armature only)
# - camera output: how to handle the cut-list?
# - ignore meshes that are not attached to the one armature
# - animation stuff is kinda hacky
# - "clean up" the framedata so that repeated identical values are not exported as animated components
# - use material name as shader name in export, instead of bitmap filename ?
# - use Mathlib instead of custom math routines
# - better utilize Blender's new API (bone rest pose extraction, ...)

# version history:
# 0.94  2006-02-09: modified for Blender2.41, 
#                   uses Pose-module (enables export of ik without baking to IPO)
# 0.93  2006-01-20: modified for Blender2.40
# 0.92a 2005-01-26: fixed stupid bug in camera export button handler
# 0.92  2004-11-04: changed the animation export back to specifyable action
#                   changed some filehandling (mesh or anim export are now optional)
# 0.91  2004-10-06: fixed borked baseframe for bones that have no IPO that animates them
#       2004-09-10: fixed script throwing an error for improperly skinned vertices with no weights
# 0.9   2004-09-03: initial release

# Parameters :

# Helps for having the model rotated right.
# Put in BASE_MATRIX your own rotation if you need some.

BASE_MATRIX = None # matrix_rotate_x(math.pi / 2.0)

#########################################################################################
# Code starts here.

import sys, os, os.path, struct, math, string, numpy
# if you get an error here, it might be
# because you don't have Python installed.
# go to www.python.org and get python 2.4 (not necessarily the latest!)

import Blender

# HACK -- it seems that some Blender versions don't define sys.argv,
# which may crash Python if a warning occurs.
if not hasattr(sys, "argv"): sys.argv = ["???"]


# Math stuff
# some of this is not used in this version, but not thrown out yet.

def matrix2quaternion(m):
  s = math.sqrt(abs(m[0][0] + m[1][1] + m[2][2] + m[3][3]))
  if s == 0.0:
    x = abs(m[2][1] - m[1][2])
    y = abs(m[0][2] - m[2][0])
    z = abs(m[1][0] - m[0][1])
    if   (x >= y) and (x >= z): return 1.0, 0.0, 0.0, 0.0
    elif (y >= x) and (y >= z): return 0.0, 1.0, 0.0, 0.0
    else:                       return 0.0, 0.0, 1.0, 0.0
  return quaternion_normalize([
    -(m[2][1] - m[1][2]) / (2.0 * s),
    -(m[0][2] - m[2][0]) / (2.0 * s),
    -(m[1][0] - m[0][1]) / (2.0 * s),
    0.5 * s,
    ])

def quaternion_normalize(q):
  l = math.sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3])
  return q[0] / l, q[1] / l, q[2] / l, q[3] / l

def quaternion_multiply(q1, q2):
  r = [
    q2[3] * q1[0] + q2[0] * q1[3] + q2[1] * q1[2] - q2[2] * q1[1],
    q2[3] * q1[1] + q2[1] * q1[3] + q2[2] * q1[0] - q2[0] * q1[2],
    q2[3] * q1[2] + q2[2] * q1[3] + q2[0] * q1[1] - q2[1] * q1[0],
    q2[3] * q1[3] - q2[0] * q1[0] - q2[1] * q1[1] - q2[2] * q1[2],
    ]
  d = math.sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3])
  r[0] /= d
  r[1] /= d
  r[2] /= d
  r[3] /= d
  return r


def matrix_invert(m):
  det = (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
       - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
       + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]))
  if det == 0.0: return None
  det = 1.0 / det
  r = [ [
      det * (m[1][1] * m[2][2] - m[2][1] * m[1][2]),
    - det * (m[0][1] * m[2][2] - m[2][1] * m[0][2]),
      det * (m[0][1] * m[1][2] - m[1][1] * m[0][2]),
      0.0,
    ], [
    - det * (m[1][0] * m[2][2] - m[2][0] * m[1][2]),
      det * (m[0][0] * m[2][2] - m[2][0] * m[0][2]),
    - det * (m[0][0] * m[1][2] - m[1][0] * m[0][2]),
      0.0
    ], [
      det * (m[1][0] * m[2][1] - m[2][0] * m[1][1]),
    - det * (m[0][0] * m[2][1] - m[2][0] * m[0][1]),
      det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]),
      0.0,
    ] ]
  r.append([
    -(m[3][0] * r[0][0] + m[3][1] * r[1][0] + m[3][2] * r[2][0]),
    -(m[3][0] * r[0][1] + m[3][1] * r[1][1] + m[3][2] * r[2][1]),
    -(m[3][0] * r[0][2] + m[3][1] * r[1][2] + m[3][2] * r[2][2]),
    1.0,
    ])
  return r

def matrix_rotate_x(angle):
  cos = math.cos(angle)
  sin = math.sin(angle)
  return [
    [1.0,  0.0, 0.0, 0.0],
    [0.0,  cos, sin, 0.0],
    [0.0, -sin, cos, 0.0],
    [0.0,  0.0, 0.0, 1.0],
    ]

def matrix_rotate_y(angle):
  cos = math.cos(angle)
  sin = math.sin(angle)
  return [
    [cos, 0.0, -sin, 0.0],
    [0.0, 1.0,  0.0, 0.0],
    [sin, 0.0,  cos, 0.0],
    [0.0, 0.0,  0.0, 1.0],
    ]

def matrix_rotate_z(angle):
  cos = math.cos(angle)
  sin = math.sin(angle)
  return [
    [ cos, sin, 0.0, 0.0],
    [-sin, cos, 0.0, 0.0],
    [ 0.0, 0.0, 1.0, 0.0],
    [ 0.0, 0.0, 0.0, 1.0],
    ]

def matrix_rotate(axis, angle):
  vx  = axis[0]
  vy  = axis[1]
  vz  = axis[2]
  vx2 = vx * vx
  vy2 = vy * vy
  vz2 = vz * vz
  cos = math.cos(angle)
  sin = math.sin(angle)
  co1 = 1.0 - cos
  return [
    [vx2 * co1 + cos,          vx * vy * co1 + vz * sin, vz * vx * co1 - vy * sin, 0.0],
    [vx * vy * co1 - vz * sin, vy2 * co1 + cos,          vy * vz * co1 + vx * sin, 0.0],
    [vz * vx * co1 + vy * sin, vy * vz * co1 - vx * sin, vz2 * co1 + cos,          0.0],
    [0.0, 0.0, 0.0, 1.0],
    ]

  
def point_by_matrix(p, m):
  return [p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0] + m[3][0],
          p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1] + m[3][1],
          p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2] + m[3][2]]

def vector_by_matrix(p, m):
  return [p[0] * m[0][0] + p[1] * m[1][0] + p[2] * m[2][0],
          p[0] * m[0][1] + p[1] * m[1][1] + p[2] * m[2][1],
          p[0] * m[0][2] + p[1] * m[1][2] + p[2] * m[2][2]]

def vector_length(v):
  return math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])

def vector_normalize(v):
  l = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
  try:
    return v[0] / l, v[1] / l, v[2] / l
  except:
    return 1, 0, 0
  
def vector_dotproduct(v1, v2):
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]

def vector_crossproduct(v1, v2):
  return [
    v1[1] * v2[2] - v1[2] * v2[1],
    v1[2] * v2[0] - v1[0] * v2[2],
    v1[0] * v2[1] - v1[1] * v2[0],
    ]

def vector_angle(v1, v2):
  s = vector_length(v1) * vector_length(v2)
  f = vector_dotproduct(v1, v2) / s
  if f >=  1.0: return 0.0
  if f <= -1.0: return math.pi / 2.0
  return math.atan(-f / math.sqrt(1.0 - f * f)) + math.pi / 2.0

# end of math stuff.

NEXT_MATERIAL_ID = 0
class Material:
  def __init__(self, map_filename = None):
    self.ambient_r  = 255
    self.ambient_g  = 255
    self.ambient_b  = 255
    self.ambient_a  = 255
    self.diffuse_r  = 255
    self.diffuse_g  = 255
    self.diffuse_b  = 255
    self.diffuse_a  = 255
    self.specular_r = 255
    self.specular_g = 255
    self.specular_b = 255
    self.specular_a = 255
    self.shininess = 1.0
    if map_filename: self.maps_filenames = [map_filename]
    else:            self.maps_filenames = []
    
    MATERIALS[map_filename] = self
    
    global NEXT_MATERIAL_ID
    self.id = NEXT_MATERIAL_ID
    NEXT_MATERIAL_ID += 1
    

  def to_md5mesh(self):
    if self.maps_filenames:
      return self.maps_filenames[0]
    else:
      return ""
    
    
MATERIALS = {}

class Mesh:
  def __init__(self, name):
    self.name      = name
    self.submeshes = []
    
    self.next_submesh_id = 0

    
  def to_md5mesh(self):
    meshnumber=0
    buf = ""
    for submesh in self.submeshes:
      buf=buf + "mesh {\n"
      meshnumber += 1
      buf=buf + submesh.to_md5mesh()
      buf=buf + "}\n\n"

    return buf
      
class SubMesh:
  def __init__(self, mesh, material):
    self.material   = material
    self.vertices   = []
    self.faces      = []
    self.nb_lodsteps = 0
    self.springs    = []
    self.weights    = []
    
    self.next_vertex_id = 0
    self.next_weight_id = 0
    
    self.mesh = mesh
    self.name = mesh.name
    self.id = mesh.next_submesh_id
    mesh.next_submesh_id += 1
    mesh.submeshes.append(self)

  def bindtomesh (self, mesh):
    # HACK: this is needed for md5 output, for the time being...
    # appending this submesh to the specified mesh, disconnecting it from the original one
    self.mesh.submeshes.remove(self)
    self.mesh = mesh
    self.id = mesh.next_submesh_id
    mesh.next_submesh_id += 1
    mesh.submeshes.append(self)

  def generateweights(self):
    self.weights = []
    self.next_weight_id = 0
    for vert in self.vertices:
      vert.generateweights()

  def reportdoublefaces(self):
    for face in self.faces:
      for face2 in self.faces:
        if not face == face2:
          if (not face.vertex1==face2.vertex1) and (not face.vertex1==face2.vertex2) and (not face.vertex1==face2.vertex3):
            return
          if (not face.vertex2==face2.vertex1) and (not face.vertex2==face2.vertex2) and (not face.vertex2==face2.vertex3):
            return
          if (not face.vertex3==face2.vertex1) and (not face.vertex3==face2.vertex2) and (not face.vertex3==face2.vertex3):
            return
          print "doubleface! %s %s" % (face, face2)
          
  def to_md5mesh(self):
    self.generateweights()

    self.reportdoublefaces()
    
    buf="\tshader \"%s\"\n\n" % (self.material.to_md5mesh())
    if len(self.weights) == 0:
      buf=buf + "\tnumverts 0\n"
      buf=buf + "\n\tnumtris 0\n"
      buf=buf + "\n\tnumweights 0\n"
      return buf
    
    # output vertices
    buf=buf + "\tnumverts %i\n" % (len(self.vertices))
    vnumber=0
    for vert in self.vertices:
      buf=buf + "\tvert %i %s\n" % (vnumber, vert.to_md5mesh())
      vnumber += 1
    
    # output faces
    buf=buf + "\n\tnumtris %i\n" % (len(self.faces))
    facenumber=0
    for face in self.faces:
      buf=buf + "\ttri %i %s\n" % (facenumber, face.to_md5mesh())
      facenumber += 1
      
    # output weights
    buf=buf + "\n\tnumweights %i\n" % (len(self.weights))
    weightnumber=0
    for weight in self.weights:
      buf=buf + "\tweight %i %s\n" % (weightnumber, weight.to_md5mesh())
      weightnumber += 1
      
    return buf



class Vertex:
  def __init__(self, submesh, loc, normal):
    self.loc    = loc
    self.normal = normal
    self.collapse_to         = None
    self.face_collapse_count = 0
    self.maps       = []
    self.influences = []
    self.weights = []
    self.weight = None
    self.firstweightindx = 0
    self.cloned_from = None
    self.clones      = []
    
    self.submesh = submesh
    self.id = submesh.next_vertex_id
    submesh.next_vertex_id += 1
    submesh.vertices.append(self)
    
  def generateweights(self):
    self.firstweightindx = self.submesh.next_weight_id
    for influence in self.influences:
      weightindx = self.submesh.next_weight_id
      self.submesh.next_weight_id += 1
      newweight = Weight(influence.bone, influence.weight, self, weightindx, self.loc[0], self.loc[1], self.loc[2])
      self.submesh.weights.append(newweight)
      self.weights.append(newweight)

  def to_md5mesh(self):
    if self.maps:
      buf = self.maps[0].to_md5mesh()
    else:
      buf = "( %f %f )" % (self.loc[0], self.loc[1])
    buf = buf + " %i %i" % (self.firstweightindx, len(self.influences))
    return buf
  
class Map:
  def __init__(self, u, v):
    self.u = u
    self.v = v
    

  def to_md5mesh(self):
    buf = "( %f %f )" % (self.u, self.v)
    return buf


class Weight:
  def __init__(self, bone, weight, vertex, weightindx, x, y, z):
    self.bone = bone
    self.weight = weight
    self.vertex = vertex
    self.indx = weightindx
    invbonematrix = matrix_invert(self.bone.matrix)
    self.x, self.y, self.z = point_by_matrix ((x, y, z), invbonematrix)
    
  def to_md5mesh(self):
    buf = "%i %f ( %f %f %f )" % (self.bone.id, self.weight, self.x*scale, self.y*scale, self.z*scale)
    return buf


class Influence:
  def __init__(self, bone, weight):
    self.bone   = bone
    self.weight = weight
    

class Face:
  def __init__(self, submesh, vertex1, vertex2, vertex3):
    self.vertex1 = vertex1
    self.vertex2 = vertex2
    self.vertex3 = vertex3
    
    self.can_collapse = 0
    
    self.submesh = submesh
    submesh.faces.append(self)
    

  def to_md5mesh(self):
    buf = "%i %i %i" % (self.vertex1.id, self.vertex3.id, self.vertex2.id)
    return buf

  
class Skeleton:
  def __init__(self, MD5Version = 10, commandline = ""):
    self.bones = []
    self.MD5Version = MD5Version
    self.commandline = commandline
    self.next_bone_id = 0
    

  def to_md5mesh(self, numsubmeshes):
    buf = "MD5Version %i\n" % (self.MD5Version)
    buf = buf + "commandline \"%s\"\n\n" % (self.commandline)
    buf = buf + "numJoints %i\n" % (self.next_bone_id)
    buf = buf + "numMeshes %i\n\n" % (numsubmeshes)
    buf = buf + "joints {\n"
    for bone in self.bones:
      buf = buf + bone.to_md5mesh()
    buf = buf + "}\n\n"
    return buf


BONES = {}

class Bone:
  def __init__(self, skeleton, parent, name, mat,pmat, theboneobj):
    self.parent = parent #Bone
    self.name   = name   #string
    self.children = []   #list of Bone objects
    self.theboneobj = theboneobj #Blender.Armature.Bone
    # HACK: this flags if the bone is animated in the one animation that we export
    self.is_animated = 0  # = 1, if there is an ipo that animates this bone

    self.matrix = mat
    self.pmatrix = pmat # parentspace matrix
    if parent:
      parent.children.append(self)
    
    self.skeleton = skeleton
    self.id = skeleton.next_bone_id
    skeleton.next_bone_id += 1
    skeleton.bones.append(self)
    
    BONES[name] = self


  def to_md5mesh(self):
    buf= "\t\"%s\"\t" % (self.name)
    parentindex = -1
    if self.parent:
        parentindex=self.parent.id
    buf=buf+"%i " % (parentindex)
    
    pos1, pos2, pos3= self.matrix[3][0], self.matrix[3][1], self.matrix[3][2]
    buf=buf+"( %f %f %f ) " % (pos1*scale, pos2*scale, pos3*scale)
    #qx, qy, qz, qw = matrix2quaternion(self.matrix)
    #if qw<0:
    #    qx = -qx
    #    qy = -qy
    #    qz = -qz
    m = self.matrix
    bquat = self.matrix.toQuat()
    bquat.normalize()
    qx = bquat.x
    qy = bquat.y
    qz = bquat.z
    if bquat.w > 0:
        qx = -qx
        qy = -qy
        qz = -qz
    buf=buf+"( %f %f %f )\t\t// " % (qx, qy, qz)
    if self.parent:
        buf=buf+"%s" % (self.parent.name)    
    
    buf=buf+"\n"
    return buf

class MD5Animation:
  def __init__(self, md5skel, MD5Version = 10, commandline = ""):
    self.framedata    = [] # framedata[boneid] holds the data for each frame
    self.bounds       = []
    self.baseframe    = []
    self.skeleton     = md5skel
    self.boneflags    = []  # stores the md5 flags for each bone in the skeleton
    self.boneframedataindex = [] # stores the md5 framedataindex for each bone in the skeleton
    self.MD5Version   = MD5Version
    self.commandline  = commandline
    self.numanimatedcomponents = 0
    self.framerate    = 24
    self.numframes    = 0
    for b in self.skeleton.bones:
      self.framedata.append([])
      self.baseframe.append([])
      self.boneflags.append(0)
      self.boneframedataindex.append(0)
      
  def to_md5anim(self):
    currentframedataindex = 0
    for bone in self.skeleton.bones:
      if (len(self.framedata[bone.id])>0):
        if (len(self.framedata[bone.id])>self.numframes):
          self.numframes=len(self.framedata[bone.id])
        (x,y,z),(qw,qx,qy,qz) = self.framedata[bone.id][0]
        self.baseframe[bone.id]= (x*scale,y*scale,z*scale,qx,qy,qz)
        self.boneframedataindex[bone.id]=currentframedataindex
        self.boneflags[bone.id] = 63
        currentframedataindex += 6
        self.numanimatedcomponents = currentframedataindex
      else:
        rot=bone.pmatrix.toQuat().normalize()
        qx=rot.x
        qy=rot.y
        qz=rot.z
        if rot.w > 0:
            qx = -qx
            qy = -qy
            qz = -qz            
        self.baseframe[bone.id]= (bone.pmatrix[3][0]*scale, bone.pmatrix[3][1]*scale, bone.pmatrix[3][2]*scale, qx, qy, qz)
        
    buf = "MD5Version %i\n" % (self.MD5Version)
    buf = buf + "commandline \"%s\"\n\n" % (self.commandline)
    buf = buf + "numFrames %i\n" % (self.numframes)
    buf = buf + "numJoints %i\n" % (len(self.skeleton.bones))
    buf = buf + "frameRate %i\n" % (self.framerate)
    buf = buf + "numAnimatedComponents %i\n\n" % (self.numanimatedcomponents)
    buf = buf + "hierarchy {\n"

    for bone in self.skeleton.bones:
      parentindex = -1
      flags = self.boneflags[bone.id]
      framedataindex = self.boneframedataindex[bone.id]
      if bone.parent:
        parentindex=bone.parent.id
      buf = buf + "\t\"%s\"\t%i %i %i\t//" % (bone.name, parentindex, flags, framedataindex)
      if bone.parent:
        buf = buf + " " + bone.parent.name
      buf = buf + "\n"
    buf = buf + "}\n\n"

    buf = buf + "bounds {\n"
    for b in self.bounds:
      buf = buf + "\t( %f %f %f ) ( %f %f %f )\n" % (b)
    buf = buf + "}\n\n"

    buf = buf + "baseframe {\n"
    for b in self.baseframe:
      buf = buf + "\t( %f %f %f ) ( %f %f %f )\n" % (b)
    buf = buf + "}\n\n"

    for f in range(0, self.numframes):
      buf = buf + "frame %i {\n" % (f)
      for b in self.skeleton.bones:
        if (len(self.framedata[b.id])>0):
          (x,y,z),(qw,qx,qy,qz) = self.framedata[b.id][f]
          if qw>0:
            qx,qy,qz = -qx,-qy,-qz
          buf = buf + "\t%f %f %f %f %f %f\n" % (x*scale, y*scale, z*scale, qx,qy,qz)
      buf = buf + "}\n\n"
      
    return buf
  
  def addkeyforbone(self, boneid, time, loc, rot):
    # time is ignored. the keys are expected to come in sequentially
    # it might be useful for future changes or modifications for other export formats
    self.framedata[boneid].append((loc, rot))
    return

def getIpoValue(ipo, value, time):
  ipocurves = ipo.getCurves()
  #return ipo.getCurve(value).evaluate(time)

  for i in range(len(ipocurves)):
    if value == ipocurves[i].getName():
      return ipo.getCurveCurval(i)
  print "Error: ipo %s has no ipo curve for %s!" %(ipo, value)

def getminmax(listofpoints):
  if len(listofpoints) == 0: return ([0,0,0],[0,0,0])
  min = [listofpoints[0][0], listofpoints[0][1], listofpoints[0][2]]
  max = [listofpoints[0][0], listofpoints[0][1], listofpoints[0][2]]
  if len(listofpoints)>1:
    for i in range(1, len(listofpoints)):
      if listofpoints[i][0]>max[0]: max[0]=listofpoints[i][0]
      if listofpoints[i][1]>max[1]: max[1]=listofpoints[i][1]
      if listofpoints[i][2]>max[2]: max[2]=listofpoints[i][2]
      if listofpoints[i][0]<min[0]: min[0]=listofpoints[i][0]
      if listofpoints[i][1]<min[1]: min[1]=listofpoints[i][1]
      if listofpoints[i][2]<min[2]: min[2]=listofpoints[i][2]
  return (min, max)

def generateboundingbox(objects, md5animation, framerange):
  scene = Blender.Scene.getCurrent()
  context = scene.getRenderingContext()
  for i in range(framerange[0], framerange[1]+1):
    corners = []
    context.currentFrame(i)
    scene.makeCurrent()
    for obj in objects:
      data = obj.getData()
      if (type(data) is Blender.Types.NMeshType) and data.faces:
        obj.makeDisplayList()
        (lx, ly, lz) = obj.getLocation()
        bbox = obj.getBoundBox()
        #matrix = obj.getMatrix()
        matrix = [[1.0,  0.0, 0.0, 0.0],
          [0.0,  1.0, 0.0, 0.0],
          [0.0,  1.0, 1.0, 0.0],
          [0.0,  0.0, 0.0, 1.0],
          ]
        if BASE_MATRIX: matrix = BASE_MATRIX * matrix
        for v in bbox:
          corners.append(point_by_matrix (v, matrix))
    (min, max) = getminmax(corners)
    md5animation.bounds.append((min[0]*scale, min[1]*scale, min[2]*scale, max[0]*scale, max[1]*scale, max[2]*scale))
  
def export(rangestart, rangeend, action_name):
  # Get the scene
  scene = Blender.Scene.getCurrent()
  context = scene.getRenderingContext()

  action_dict = Blender.Armature.NLA.GetActions()
  if not action_dict.has_key(action_name):
    print "Specified action does not exist: %s " % (action_name)
    print "No md5anim will be generated"
    arm_action = None
  else:
    arm_action = action_dict[action_name]
    ipomap = arm_action.getAllChannelIpos()

  # Export skeleton (=armature)
  skeleton = Skeleton()
  for obj in Blender.Object.Get():
    data = obj.getData()
    if type(data) is Blender.Types.ArmatureType:
      thearmature = obj
      matrix = obj.getMatrix('worldspace')
      if BASE_MATRIX: matrix = BASE_MATRIX * matrix
      
      def treat_bone(b, parent = None):
        if (parent and not b.parent.name==parent.name):
          return #only catch direct children

        mat = Blender.Mathutils.Matrix(b.matrix['ARMATURESPACE'])*matrix
        #pmat is bone's matrix in parent space
        #pmat = Blender.Mathutils.Matrix(b.matrix['BONESPACE'])*matrix.toQuat().toMatrix() #god this api is ugly, bonespace, armaturespace matrices have different dimensions
        if (parent):
          pmat = Blender.Mathutils.Matrix(b.matrix['ARMATURESPACE'])*Blender.Mathutils.Matrix(parent.theboneobj.matrix['ARMATURESPACE']).invert()
        else:
          pmat = Blender.Mathutils.Matrix(b.matrix['ARMATURESPACE'])*matrix
        bone = Bone(skeleton, parent, b.name, mat, pmat, b)
       
        if (b.children):
          for child in b.children: treat_bone(child, bone)
        
      for b in data.bones.values():
        if (not b.parent):  # only treat root bones, treat_bone takes care of children
          treat_bone(b)
      
      # Only one armature (we break here so this first armature in the scene is the only one we export)
      break
    
    
  # Export Mesh data
  
  meshes = []
  for obj in Blender.Object.Get():
    data = obj.getData()
    if (type(data) is Blender.Types.NMeshType) and data.faces:
      mesh = Mesh(obj.name)
      print "Processing mesh: ", obj.name
      meshes.append(mesh)
      
      matrix = obj.getMatrix('worldspace')
      if BASE_MATRIX: matrix = BASE_MATRIX * matrix
      
      faces = data.faces
      while faces:
        image          = faces[0].image
        image_filename = image and image.filename
        material       = MATERIALS.get(image_filename) or Material(image_filename)
        
        # TODO add material color support here
        
        submesh  = SubMesh(mesh, material)
        vertices = {}
        for face in faces[:]:
          # der_ton: i added this check to make sure a face has at least 3 vertices.
          # i've seen several models that threw an error in the p3 = face.v[2].co line
          if len(face.v)<3: # throw away faces that have less than 3 vertices
            faces.remove(face)
          elif face.v[0].co == face.v[1].co:
            faces.remove(face)
          elif face.v[0].co == face.v[2].co:
            faces.remove(face)
          elif face.v[1].co == face.v[2].co:
            faces.remove(face)
          elif (face.image and face.image.filename) == image_filename:
            faces.remove(face)
            
            if not face.smooth:
              p1 = face.v[0].co
              p2 = face.v[1].co
              p3 = face.v[2].co
              normal = vector_normalize(vector_by_matrix(vector_crossproduct(
                [p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]],
                [p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]],
                ), matrix))
              
            face_vertices = []
            for i in range(len(face.v)):
              vertex = vertices.get(face.v[i].index)
              if not vertex:
                coord  = point_by_matrix (face.v[i].co, matrix)
                if face.smooth: normal = vector_normalize(vector_by_matrix(face.v[i].no, matrix))
                vertex  = vertices[face.v[i].index] = Vertex(submesh, coord, normal)

                influences = data.getVertexInfluences(face.v[i].index)
                if not influences:
                  print "There is a vertex without attachment to a bone in mesh: " + mesh.name
                # sum of influences is not always 1.0 in Blender ?!?!
                sum = 0.0
                for bone_name, weight in influences: sum += weight
                
                for bone_name, weight in influences:
                  if sum!=0:
                    try:
                        vertex.influences.append(Influence(BONES[bone_name], weight / sum))
                    except:
                        continue
                  else: # we have a vertex that is probably not skinned. export anyway
                    try:
                        vertex.influences.append(Influence(BONES[bone_name], weight))
                    except:
                        continue
                  
              elif not face.smooth:
                # We cannot share vertex for non-smooth faces, since Cal3D does not
                # support vertex sharing for 2 vertices with different normals.
                # => we must clone the vertex.
                
                old_vertex = vertex
                vertex = Vertex(submesh, vertex.loc, normal)
                vertex.cloned_from = old_vertex
                vertex.influences = old_vertex.influences
                old_vertex.clones.append(vertex)
                
              if data.hasFaceUV():
                uv = [face.uv[i][0], 1.0 - face.uv[i][1]]
                if not vertex.maps: vertex.maps.append(Map(*uv))
                elif (vertex.maps[0].u != uv[0]) or (vertex.maps[0].v != uv[1]):
                  # This vertex can be shared for Blender, but not for MD5
                  # MD5 does not support vertex sharing for 2 vertices with
                  # different UV texture coodinates.
                  # => we must clone the vertex.
                  
                  for clone in vertex.clones:
                    if (clone.maps[0].u == uv[0]) and (clone.maps[0].v == uv[1]):
                      vertex = clone
                      break
                  else: # Not yet cloned...
                    old_vertex = vertex
                    vertex = Vertex(submesh, vertex.loc, vertex.normal)
                    vertex.cloned_from = old_vertex
                    vertex.influences = old_vertex.influences
                    vertex.maps.append(Map(*uv))
                    old_vertex.clones.append(vertex)
                    
              face_vertices.append(vertex)
              
            # Split faces with more than 3 vertices
            for i in range(1, len(face.v) - 1):
              Face(submesh, face_vertices[0], face_vertices[i], face_vertices[i + 1])
              


  # Export animations

  ANIMATIONS = {}
  if arm_action:
    animation = ANIMATIONS[arm_action.getName()] = MD5Animation(skeleton)
    arm_action.setActive(thearmature)
    armData = thearmature.getData()
    
    currenttime = rangestart # currenttime is blender's framenumber that we iterate over, getting also the frames between keyframes
                    # time is the "realtime", stored in keyframe objects and in animation.duration
    while currenttime <=rangeend:
      context.currentFrame(int(currenttime))
      # Needed to update IPO's value, but probably not the best way for that...
      scene.makeCurrent()
      Blender.Set("curframe", int(currenttime))
      Blender.Window.Redraw()
      # Convert time units from Blender's frame (starting at 1) to seconds
      time = (currenttime - 1.0) / 24.0 #(assuming default 24fps for md5 anim)
      pose = thearmature.getPose()
      
      for bonename in armData.bones.keys():
        posebonemat = Blender.Mathutils.Matrix(pose.bones[bonename].poseMatrix)

        try:
          bone  = BONES[bonename] #look up md5bone
        except:
          print "found a posebone animating a bone that is not part of the exported armature: ", ipo.getName()
          continue
        if bone.parent: # need parentspace-matrix
          parentposemat = Blender.Mathutils.Matrix(pose.bones[bone.parent.name].poseMatrix)
          posebonemat = posebonemat*parentposemat.invert()
        else:
          posebonemat = posebonemat*thearmature.getMatrix('worldspace')
        loc = [posebonemat[3][0],
            posebonemat[3][1],
            posebonemat[3][2],
            ]
        rot = posebonemat.toQuat().normalize()
        rot = [rot.w,rot.x,rot.y,rot.z]

        animation.addkeyforbone(bone.id, time, loc, rot)
      currenttime += 1


  #if not os.path.exists(SAVE_TO_DIR): os.makedirs(SAVE_TO_DIR)

  # here begins md5mesh and anim output
  # this is how it works
  # first the skeleton is output, using the data that was collected by the above code in this export function
  # then the mesh data is output (into the same md5mesh file)


  # HACK: save all submeshes in the first mesh
  if len(meshes)>1:
    for mesh in range (1, len(meshes)):
      for submesh in meshes[mesh].submeshes:
        submesh.bindtomesh(meshes[0])
  if (md5mesh_filename.val != ""):
    try:
      file = open(md5mesh_filename.val, 'w')
    except IOError, (errno, strerror):
      errmsg = "IOError #%s: %s" % (errno, strerror)
    buffer = skeleton.to_md5mesh(len(meshes[0].submeshes))
    #for mesh in meshes:
    buffer = buffer + meshes[0].to_md5mesh()
    file.write(buffer)
    file.close()
    print "saved mesh to " + md5mesh_filename.val
  else:
    print "No md5mesh file was generated."

##  # this piece of code might be useful for someone, so it is left here in comments
##  # this generates a md5anim file that simply contains the idle pose
##  try:
##    file = open("idlepose.md5anim"), 'w')
##  except IOError, (errno, strerror):
##    errmsg = "IOError #%s: %s" % (errno, strerror)
##  animation = skeleton.to_idlemd5anim() # animation is a MD5Animation object, not an Animation
##  buffer = animation.to_md5anim()
##  file.write(buffer)
##  file.close()

  if len(ANIMATIONS)>0 and md5anim_filename.val != "":
    anim = ANIMATIONS.values()[0]
    #animfilename = anim.name + ".md5anim"
    try:
      file = open(md5anim_filename.val, 'w')
    except IOError, (errno, strerror):
      errmsg = "IOError #%s: %s" % (errno, strerror)
##    if len(themd5anim.channels)>0:
##      ##rangestart = themd5anim.channels[0].starttime*themd5anim.channels[1].framerate
##      ##rangeend = themd5anim.channels[0].endtime*themd5anim.channels[1].framerate
##      rangeend = len(themd5anim.channels[0].keys)
##      for ch in themd5anim.channels:
##        ##chstart = ch.starttime*ch.framerate
##        chend = len(ch.keys)
##        ##if rangestart>chstart: rangestart = chstart
##        if rangeend<chend: rangeend = chend
      
    objects = []
    for submesh in meshes[0].submeshes:
      if len(submesh.weights) > 0:
        obj = Blender.Object.Get(submesh.name)
        objects.append (obj)
    generateboundingbox(objects, anim, [rangestart, rangeend])
    buffer = anim.to_md5anim()
    file.write(buffer)
    file.close()
    print "saved animation to " + md5anim_filename.val
  else:
    print "No md5anim was generated."

class MD5CameraV10:
  def __init__(self, framerate):
    self.commandline = ""
    self.framerate = framerate
    self.cuts = []
    self.frames = []
  def to_md5camera(self):
    buf = "MD5Version 10\n"
    buf = buf + "commandline \"%s\"\n\n" % (self.commandline)
    buf = buf + "numFrames %i\n" % (len(self.frames))
    buf = buf + "frameRate %i\n" % (self.framerate)
    buf = buf + "numCuts %i\n\n" % (len(self.cuts))
    buf = buf + "cuts {\n"
    for c in self.cuts:
      buf = buf + "\t%i\n" % (c)
    buf = buf + "}\n\n"
    
    buf = buf + "camera {\n"
    for f in self.frames:
      buf = buf + "\t( %f %f %f ) ( %f %f %f ) %f\n" % (f)
    buf = buf + "}\n\n"
    return buf
    
def export_camera(rangestart, rangeend):
  cams = Blender.Camera.Get()
  scene = Blender.Scene.getCurrent()
  context = scene.getRenderingContext()
  if len(cams)==0: return None
  camobj = Blender.Object.Get(cams[0].name)
  
  #generate the animation
  themd5cam = MD5CameraV10(context.framesPerSec())
  for i in range(rangestart, rangeend+1):
    context.currentFrame(i)    
    scene.makeCurrent()
    Blender.Redraw() # apparently this has to be done to update the object's matrix. Thanks theeth for pointing that out
    loc = camobj.getLocation()
    m1 = camobj.getMatrix('worldspace')
    
    # this is because blender cams look down their negative z-axis and "up" is y
    # doom3 cameras look down their x axis, "up" is z
    m2 = [[-m1[2][0], -m1[2][1], -m1[2][2], 0.0], [-m1[0][0], -m1[0][1], -m1[0][2], 0.0], [m1[1][0], m1[1][1], m1[1][2], 0.0], [0,0,0,1]]
    qx, qy, qz, qw = matrix2quaternion(m2)
    if qw>0:
      qx,qy,qz = -qx,-qy,-qz
    fov = 2 * math.atan(16/cams[0].getLens())*180/math.pi
    themd5cam.frames.append((loc[0]*scale, loc[1]*scale, loc[2]*scale, qx, qy, qz, fov))
    
  try:
    file = open(md5camanim_filename.val, 'w')
  except IOError, (errno, strerror):
    errmsg = "IOError #%s: %s" % (errno, strerror)
  buffer = themd5cam.to_md5camera()
  file.write(buffer)
  file.close()
  print "saved md5animation to " + md5camanim_filename.val


print "\nAvailable actions:"
print Blender.Armature.NLA.GetActions().keys()

draw_busy_screen = 0
EVENT_NOEVENT = 1
EVENT_EXPORT = 2
EVENT_QUIT = 3
EVENT_MESHFILENAME = 4
EVENT_ANIMFILENAME = 5
EVENT_MESHFILENAME_STRINGBUTTON = 6
EVENT_ANIMFILENAME_STRINGBUTTON = 7
EVENT_CAMEXPORT = 8
EVENT_CAM_ANIMFILENAME = 10
md5mesh_filename = Blender.Draw.Create("")
md5anim_filename = Blender.Draw.Create("")
md5camanim_filename = Blender.Draw.Create("")
if len(Blender.Armature.NLA.GetActions().keys())>0:
    firstaction_name = Blender.Armature.NLA.GetActions().keys()[0]
else:
    firstaction_name = ""
export_animation = Blender.Draw.Create(firstaction_name)

## this is not used in this version, but might come back or might be useful for people who modify this script
## currently, the first action that is found will get exported
# This is the name of the animation that should get exported
#export_animation = Blender.Draw.Create("Action")


# this is a scale factor for md5 exporting. scaling with BASE_MATRIX won't work correctly
# setting this to 1 (no scaling) might help if your animation looks distorted
scale_slider = Blender.Draw.Create(1.0)
scale = 1.0

scene = Blender.Scene.getCurrent()
context = scene.getRenderingContext()

startframe_slider = Blender.Draw.Create(context.startFrame())
endframe_slider = Blender.Draw.Create(context.endFrame())
framespersecond = context.framesPerSec()

######################################################
# Callbacks for Window functions
######################################################
def md5meshname_callback(filename):
  global md5mesh_filename
  md5mesh_filename.val=filename

def md5animname_callback(filename):
  global md5anim_filename
  md5anim_filename.val=filename

def md5camanimname_callback(filename):
  global md5camanim_filename
  md5camanim_filename.val=filename
  
######################################################
# GUI Functions
######################################################
def handle_event(evt, val):
  if evt == Blender.Draw.ESCKEY:
    Blender.Draw.Exit()
    return

def handle_button_event(evt):
  global EVENT_NOEVENT, EVENT_CAMEXPORT, EVENT_EXPORT, EVENT_QUIT, EVENT_MESHFILENAME, EVENT_ANIMFILENAME, EVENT_MESHFILENAME_STRINGBUTTON, EVENT_ANIMFILENAME_STRINGBUTTON
  global EVENT_CAM_MESHFILENAME, EVENT_CAM_ANIMFILENAME
  global draw_busy_screen, md5mesh_filename, md5anim_filename, scale_slider, scale
  global export_animation
  global startframe_slider, endframe_slider
  global md5camanim_filename
  if evt == EVENT_EXPORT:
    if md5anim_filename.val == "" and md5mesh_filename.val == "": return
    scale = scale_slider.val
    draw_busy_screen = 1
    Blender.Draw.Draw()
    export(startframe_slider.val, endframe_slider.val, export_animation.val)
    draw_busy_screen = 0
    Blender.Draw.Redraw(1)
    return
  if evt == EVENT_QUIT:
    Blender.Draw.Exit()
  if evt == EVENT_MESHFILENAME:
    Blender.Window.FileSelector(md5meshname_callback, "Select md5mesh file...")
    Blender.Draw.Redraw(1)
  if evt == EVENT_ANIMFILENAME:
    Blender.Window.FileSelector(md5animname_callback, "Select md5anim file...")
    Blender.Draw.Redraw(1)
  if evt == EVENT_CAM_ANIMFILENAME:
    Blender.Window.FileSelector(md5camanimname_callback, "Select md5anim file...")
    Blender.Draw.Redraw(1)
  if evt == EVENT_CAMEXPORT:
    if md5camanim_filename.val == "": return
    scale = scale_slider.val
    draw_busy_screen = 1
    Blender.Draw.Draw()
    export_camera(startframe_slider.val, endframe_slider.val)
    draw_busy_screen = 0
    Blender.Draw.Redraw(1)
  #wow, I just saw Dane's and HCL's C64 demo "Cycle", released today (2004-02-29) at Floppy2004, got voted first place. Simply AMAZING. I'll quickly finish this GUI crap and look into their plasma code...

def show_gui():
  global EVENT_NOEVENT, EVENT_CAMEXPORT, EVENT_EXPORT, EVENT_QUIT, EVENT_MESHFILENAME, EVENT_ANIMFILENAME, EVENT_MESHFILENAME_STRINGBUTTON, EVENT_ANIMFILENAME_STRINGBUTTON
  global draw_busy_screen, md5mesh_filename, md5anim_filename, scale_slider
  global export_animation
  global EVENT_CAM_MESHFILENAME, EVENT_CAM_ANIMFILENAME
  global md5camanim_filename
  global startframe_slider, endframe_slider
  button_width = 240
  browsebutton_width = 60
  button_height = 25
  if draw_busy_screen == 1:
    Blender.BGL.glClearColor(0.3,0.3,0.3,1.0)
    Blender.BGL.glClear(Blender.BGL.GL_COLOR_BUFFER_BIT)
    Blender.BGL.glColor3f(1,1,1)
    Blender.BGL.glRasterPos2i(20,25)
    Blender.Draw.Text("Please wait while exporting...")
    return
  Blender.BGL.glClearColor(0.6,0.6,0.6,1.0)
  Blender.BGL.glClear(Blender.BGL.GL_COLOR_BUFFER_BIT)
  Blender.Draw.Button("Export!", EVENT_EXPORT, 20, 2*button_height, button_width, button_height, "Start the MD5-export")
  Blender.Draw.Button("Quit", EVENT_QUIT, 20, button_height, button_width, button_height, "Quit this script")
  Blender.Draw.Button("Browse...", EVENT_MESHFILENAME, 21+button_width-browsebutton_width, 4*button_height, browsebutton_width, button_height, "Specify md5mesh-file")
  Blender.Draw.Button("Browse...", EVENT_ANIMFILENAME, 21+button_width-browsebutton_width, 3*button_height, browsebutton_width, button_height, "Specify md5anim-file")
  md5mesh_filename = Blender.Draw.String("MD5Mesh file:", EVENT_MESHFILENAME_STRINGBUTTON, 20, 4*button_height, button_width-browsebutton_width, button_height, md5mesh_filename.val, 255, "MD5Mesh-File to generate")
  md5anim_filename = Blender.Draw.String("MD5Anim file:", EVENT_ANIMFILENAME_STRINGBUTTON, 20, 3*button_height, button_width-browsebutton_width, button_height, md5anim_filename.val, 255, "MD5Anim-File to generate")
  scale_slider = Blender.Draw.Slider("Scale:", EVENT_NOEVENT, 20, 12*button_height, button_width, button_height, scale_slider.val, 0.01, 10.0, 0, "Adjust the size factor of the exported object and camera anim")
  export_animation = Blender.Draw.String("Export Action:", EVENT_NOEVENT, 20, 5*button_height, button_width, button_height, export_animation.val, 255, "Specify the name of the Action that should be exported (IPO name prefix)")
  Blender.Draw.Button("Export Camera!", EVENT_CAMEXPORT, 20, 7*button_height, button_width, button_height, "Start the Camera-export")
  Blender.Draw.Button("Browse...", EVENT_CAM_ANIMFILENAME, 21+button_width-browsebutton_width, 8*button_height, browsebutton_width, button_height, "Specify MD5Camera-file")
  md5camanim_filename = Blender.Draw.String("MD5Camera file:", EVENT_NOEVENT, 20, 8*button_height, button_width-browsebutton_width, button_height, md5camanim_filename.val, 255, "MD5Camera-File to generate")
  startframe_slider = Blender.Draw.Slider("First frame:", EVENT_NOEVENT, 20, 11*button_height, button_width, button_height, startframe_slider.val, 1, 500, 0, "First frame of animation to export")
  endframe_slider = Blender.Draw.Slider("Last frame:", EVENT_NOEVENT, 20, 10*button_height, button_width, button_height, endframe_slider.val, 1, 500, 0, "The last frame of animation to export")

Blender.Draw.Register (show_gui, handle_event, handle_button_event)