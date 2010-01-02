#!BPY

"""
Name: 'Trezkers model format (.tmf)...'
Blender: 244
Group: 'Export'
Tooltip: 'TMF exporter'
"""
import Blender
import BPyMesh

def write(filename):
	start = Blender.sys.time()
	if not filename.lower().endswith('.tmf'):
		filename += '.tmf'
	
	scn= Blender.Scene.GetCurrent()
	ob= scn.objects.active
	if not ob:
		Blender.Draw.PupMenu('Error%t|Select 1 active object')
		return
	
	file = open(filename, 'wb')
	
	mesh = BPyMesh.getMeshFromObject(ob, None, True, False, scn)
	if not mesh:
		Blender.Draw.PupMenu('Error%t|Could not get mesh data from active object')
		return
	
	mesh.transform(ob.matrixWorld)
	
	
	file = open(filename, "wb")

	#Write vertex coords and normals
	file.write("C " + `len(mesh.verts)` + "\n")
	for v in mesh.verts:
		file.write('%.6f %.6f %.6f ' % tuple(v.co))
		file.write('\n')

	file.write("N " + `len(mesh.verts)` + "\n")
	for v in mesh.verts:
		file.write('%.6f %.6f %.6f ' % tuple(v.no))
		file.write('\n')

	#Process faces
	faces = len(mesh.faces)
	data = ""
	uvdata = ""
	for face in mesh.faces:
		if len(face.v) < 3:
			# discard
			faces = faces - 1
		elif len(face.v) < 4:
			# Already a triangle, add it to the data, do not change the count
			data = data + `face.v[0].index` + " " + `face.v[1].index` + " " + `face.v[2].index` + "\n"
		else:
			# this one is a quad
			# Break it up into two triangles
			# Hence one additional face
			faces = faces + 1
			data = data + `face.v[0].index` + " " + `face.v[1].index` + " " + `face.v[3].index` + "\n"
			data = data + `face.v[1].index` + " " + `face.v[2].index` + " " + `face.v[3].index` + "\n"
	# Now I can write the header with the correct face count, and then the data
	file.write("F " + `faces` + "\n")
	file.write(data)

	uvs = faces*3
	file.write("T " + `uvs` + "\n")
	for face in mesh.faces:
		if len(face.v) < 3:
			print 'no face'
		elif len(face.v) < 4:
			# Already a triangle
			for v in face.uv:
				file.write('%.6f %.6f ' % tuple(v))
				file.write('\n')
		else:
			# this one is a quad
			# Break it up into two triangles
			file.write('%.6f %.6f ' % tuple(face.uv[0]))
			file.write('\n')
			file.write('%.6f %.6f ' % tuple(face.uv[1]))
			file.write('\n')
			file.write('%.6f %.6f ' % tuple(face.uv[3]))
			file.write('\n')

			file.write('%.6f %.6f ' % tuple(face.uv[1]))
			file.write('\n')
			file.write('%.6f %.6f ' % tuple(face.uv[2]))
			file.write('\n')
			file.write('%.6f %.6f ' % tuple(face.uv[3]))
			file.write('\n')

	file.close()
	
	end = Blender.sys.time()
	message = 'Successfully exported "%s" in %.4f seconds' % ( Blender.sys.basename(filename), end-start)
	print message


Blender.Window.FileSelector(write, 'TMF Export', Blender.sys.makename(ext='.tmf'))
