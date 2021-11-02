project(alledge)

file(GLOB alledge_SRC CONFIGURE_DEPENDS "src/*.cpp")

add_compile_options(-DGL_GLEXT_PROTOTYPES)

add_library(alledge ${alledge_SRC})

file(GLOB files "examples/*.cpp")
foreach(file ${files})
  get_filename_component(n ${file} NAME_WE)
  add_executable(example_${n} ${file})
  target_link_libraries(example_${n} alledge GL GLU glut allegro allegro_image allegro_font allegro_ttf allegro_primitives allegro_dialog)
endforeach()
