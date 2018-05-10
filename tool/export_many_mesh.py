import os
import bpy
import bmesh


# INPUT
scale      = 1
output_path = "assets/models/HOUSE.yml"
model = "HOUSE"
material = "HOUSE"
shader = "objshader"


def triangulate(obj):
    """@doc https://blender.stackexchange.com/questions/45698/triangulate-mesh-in-python - 21.04.2018"""
    
    me = obj.data
    # Get a BMesh representation
    bm = bmesh.new()
    bm.from_mesh(me)

    bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)

    # Finish up, write the bmesh back to the mesh
    bm.to_mesh(me)
    bm.free()



# Example:
# """
# vertices: 28
# """
def write_vertices_header(outfile, meshes):

    vertex_count = 0
    for mesh in meshes:

        if len(mesh.data.polygons[0].vertices) != 3:
            print("Triangulating", mesh.name)
            triangulate(mesh)

        vertex_count += len(mesh.data.vertices)


    outfile.write("vertices: {}\n".format(str(vertex_count)))


# Example:
# """
# v: -0.5 -0.5  0.5  -0.577 -0.577  0.577  0.0 0.0   150 150  150 255
# v:  0.5 -0.5  0.5   0.577 -0.577  0.577  1.0 0.0   150 150  150 255
# v:  0.5  0.5  0.5   0.577  0.577  0.577  1.0 1.0   150 150  150 255
# v: -0.5  0.5  0.5  -0.577  0.577  0.577  0.0 1.0   150 150  150 255
# """
def write_mesh_vertices(outfile, mesh):
    global scale
    
    # Get UV coordinates
    uv_dict = {}
    loops = mesh.data.loops
    if len(mesh.data.uv_layers):
        active_layer = mesh.data.uv_layers[0]
        if active_layer:
            uv_layer = mesh.data.uv_layers.active.data

            for poly in mesh.data.polygons:
                for li in poly.loop_indices:             
                    vi = loops[li].vertex_index
                    uv_dict[vi] = uv_layer[li].uv


   
    # @doc https://blender.stackexchange.com/q/4820 - 2018-05-10
#    for poly in mesh.data.polygons:
 #       for vert, loop in zip(poly.vertices, poly.loop_indices):
            
#            outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f} ".format(
 #               mesh.data.vertices[vert].co[0]*scale,
  #              mesh.data.vertices[vert].co[1]*scale,
   #             mesh.data.vertices[vert].co[2]*scale,
#
 #               mesh.data.vertices[vert].normal[0],
  #              mesh.data.vertices[vert].normal[1],
   #             mesh.data.vertices[vert].normal[2]))


    #        active_layer = False
     #       if len(mesh.data.uv_layers):
      #          active_layer = mesh.data.uv_layers[0]
       #         if active_layer:
        #            uv_layer = mesh.data.uv_layers.active.data
#
 #                   outfile.write("{:6.3f} {:6.3f}   ".format(
  #                      mesh.data.uv_layers.active.data[loop].uv[0], #U
#                     mesh.data.uv_layers.active.data[loop].uv[1])) #V


  #          outfile.write("{:3} {:3} {:3} {:3}\n".format(255,255,255,255))


    # write vertices
    for vc in mesh.data.vertices:
        outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}\n".format(
                        vc.co[0]*scale,
                        vc.co[1]*scale,
                        vc.co[2]*scale,
                        vc.normal[0],
                        vc.normal[1],
                        vc.normal[2],
                        (uv_dict.get(vc.index, [0,0])[0]),
                        (1-uv_dict.get(vc.index, [0,0])[1]),
                        255, 255, 255, 255))


# Example:
"""
mesh: innercube
    material: _default
    shader: _default
    triangles: 12
"""
def write_meshes_header(outfile, meshes):
    outfile.write("\nmeshes: "+str(len(meshes))+"\n");


def write_mesh_header(outfile, mesh):
    global material
    global shader

    # 1. Count number of meshes
    outfile.write("\nmesh: "+ mesh.name + "\n")
    outfile.write("material: " + material + "\n")
    outfile.write("shader: " + shader +"\n")
    outfile.write("triangles: {}\n".format(len(mesh.data.polygons)))


def write_mesh_triangles(outfile, mesh, offset):

    for p in mesh.data.polygons:

        outfile.write("t: {} {} {}\n".format(
            p.vertices[0]+offset,
            p.vertices[1]+offset,
            p.vertices[2]+offset
        ))

    return len(mesh.data.vertices)


if __name__ == "__main__":

    print()
    print("----- BEGIN many meshes script ------")
    print()

    # Filter out Meshes
    meshes = [o for o in bpy.data.objects if o.type == "MESH"]

    [print(m) for m in meshes]

    # Open two files, one for vertices and one for triangles
    with open(output_path+".temp_vertices", "w") as out_vertices:
        with open(output_path+".temp_meshes", "w") as out_meshes:

            write_vertices_header(out_vertices, meshes)
            write_meshes_header(out_meshes, meshes)

            offset = 0
            step = 0
            for mesh in meshes:
                offset += step
                write_mesh_vertices(out_vertices, mesh)

                write_mesh_header(out_meshes, mesh)
                step = write_mesh_triangles(out_meshes, mesh, offset)


    # COMBINE FILES
    with open(output_path+".temp_vertices", "r") as in_vertices:
        with open(output_path+".temp_meshes", "r") as in_meshes:
            with open(output_path, "w") as out_model:

                out_model.write(in_vertices.read())
                out_model.write(in_meshes.read())


    print()
    print("----- END many meshes script ------")
    print()

