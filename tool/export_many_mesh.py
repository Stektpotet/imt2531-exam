import os
import bpy
import bmesh


# INPUT
in_scale      = 1
in_output_path = "assets/models/HOUSE.yml"
in_model = "HOUSE"
in_material = "HOUSE"
in_shader = "objshader"


def hasUV(mesh):
    if len(mesh.data.tessface_uv_textures) == 0:
        return False

    if mesh.data.tessface_uv_textures.active is None:
        return False

    return True

def unpackVec3(vec3):
    return (vec3[0], vec3[1], vec3[2])

def unpackVec2(vec2):
    return (vec2[0], vec2[1])

class Vertex:
    __slot__ = ['position', 'normal', 'uv']

    def __init__(self, position, normal, uv):
        self.position = position
        self.normal = normal
        self.uv = uv



g_vertices            = []
g_vertex_dict         = {}
g_vertex_count        = 0

def resolve_mesh_triangles(mesh):
    global g_vertices
    global g_vertex_dict
    global g_vertex_count

    mesh_triangles = []

    # @doc https://docs.blender.org/api/blender_python_api_2_67_release/info_gotcha.html#upgrading-exporters-from-2-62 2018-05-09
    mesh.data.update(calc_tessface=True)

    vertexDict = {}
    
    if hasUV(mesh):
        activeUV = mesh.data.tessface_uv_textures.active.data

    for i, face in enumerate(mesh.data.tessfaces):
        is_smooth = face.use_smooth

        temp_face = []
        for j, vertIndex in enumerate(face.vertices):

            position = mesh.data.vertices[vertIndex].co
            position = unpackVec3(position)

            if is_smooth:
                normal = mesh.data.vertices[vertIndex].normal
            else:
                normal = face.normal
            normal = unpackVec3(normal)

            if hasUV(mesh):
                uv = activeUV[i].uv[j]
                uv = unpackVec2(uv)
            else:
                uv = (0.0, 0.0)

            key = position, normal, uv
            vertex_index = g_vertex_dict.get(key)

            if vertex_index is None:
                g_vertex_dict[key] = g_vertex_count
                g_vertices.append(Vertex(position=position, normal=normal, uv=uv))
                temp_face.append(g_vertex_count)
                g_vertex_count += 1
            else:
                temp_face.append(g_vertex_dict[key])

        # If triangle
        if len(temp_face) == 3:
            mesh_triangles.append(temp_face)
        # If Quad, split it up into two triangles
        else:
            mesh_triangles.append((temp_face[0], temp_face[1], temp_face[2]))
            mesh_triangles.append((temp_face[0], temp_face[2], temp_face[3]))

    return mesh_triangles



# Example:
# """
# vertices: 2334455
# v: -0.5 -0.5  0.5  -0.577 -0.577  0.577  0.0 0.0   150 150  150 255
# v:  0.5 -0.5  0.5   0.577 -0.577  0.577  1.0 0.0   150 150  150 255
# v:  0.5  0.5  0.5   0.577  0.577  0.577  1.0 1.0   150 150  150 255
# v: -0.5  0.5  0.5  -0.577  0.577  0.577  0.0 1.0   150 150  150 255
# ..........
# """
def write_vertices(outfile, vertices):
    global in_scale
    # write vertices
    outfile.write("vertices: {}\n".format(len(vertices)))
    for vert in vertices:
        outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}\n".format(
                        vert.position[0]*in_scale,
                        vert.position[1]*in_scale,
                        vert.position[2]*in_scale,
                        vert.normal[0],
                        vert.normal[1],
                        vert.normal[2],
                        vert.uv[0],
                        (1 - vert.uv[1]), # flip y axis
                        255, 255, 255, 255)) # colors


# Example:
"""
meshes: 40
"""
def write_meshes_header(outfile, meshes):
    outfile.write("\nmeshes: {}\n".format(len(meshes)));



# Example:
"""
mesh: innercube
material: _default
shader: _default
"""
def write_mesh_header(outfile, mesh):
    global in_material
    global in_shader

    # 1. Count number of meshes
    outfile.write("\nmesh: {}\n".format(mesh.name))
    outfile.write("material: {}\n".format(in_material))
    outfile.write("shader: {}\n".format(in_shader))



# Example:
"""
triangles: 5566
t: 1 2 3
t: 2 3 1
t: 6 5 4
.......
"""
def write_mesh_triangles(outfile, triangles):
    outfile.write("triangles: {}\n".format(len(triangles)))
    
    for tri in triangles:
        outfile.write("t: {} {} {}\n".format(
            tri[0],
            tri[1],
            tri[2]
        ))



if __name__ == "__main__":

    print()
    print("----- BEGIN many meshes script ------")
    print()

    # Filter out Meshes
    meshes = [o for o in bpy.data.objects if o.type == "MESH"]

    [print(m) for m in meshes]

    # Open two files, one for vertices and one for triangles
    with open(in_output_path+".temp_meshes", "w") as out_meshes:

        write_meshes_header(out_meshes, meshes)

        for mesh in meshes:
            triangles = resolve_mesh_triangles(mesh)

            write_mesh_header(out_meshes, mesh)
            write_mesh_triangles(out_meshes, triangles)

    # Write vertices
    with open(in_output_path+".temp_vertices", "w") as out_vertices:
        write_vertices(out_vertices, g_vertices)


    # COMBINE FILES
    with open(in_output_path+".temp_vertices", "r") as in_vertices:
        with open(in_output_path+".temp_meshes", "r") as in_meshes:
            with open(in_output_path, "w") as out_model:

                out_model.write(in_vertices.read())
                out_model.write(in_meshes.read())


    print()
    print("----- END many meshes script ------")
    print()

