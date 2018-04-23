import os
import bpy
import bmesh

# input
scale   = 1

# @doc https://blender.stackexchange.com/questions/45698/triangulate-mesh-in-python - 21.04.2018
def triangulate(obj):
    me = obj.data
    # Get a BMesh representation
    bm = bmesh.new()
    bm.from_mesh(me)

    bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)

    # Finish up, write the bmesh back to the mesh
    bm.to_mesh(me)
    bm.free()


def writeVertices(outfile, vertices):
    global scale
    outfile.write("vertices: {}\n".format(len(vertices)))
    for vc in vertices:
        outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}\n".format( 
                        vc.co[0]*scale,
                        vc.co[1]*scale,
                        vc.co[2]*scale,
                        vc.normal[0],
                        vc.normal[1],
                        vc.normal[2],
                        0, 0, 255, 255, 255, 255))


def writeTriangles(outfile, polygons):
    
    outfile.write("triangles: {}\n".format(len(polygons)))
    for p in polygons:
        if len(p.vertices) != 3:
            print("\nERROR, TRIANGULATE YOUR MESH!!!!")
            print("Vertexcount: ", len(p.vertices) )
            
        outfile.write("t: {} {} {}\n".format(
            p.vertices[0],
            p.vertices[1],
            p.vertices[2]
        ))

if __name__ == "__main__":

    active_object = bpy.context.active_object
    
    mesh = active_object.data

    vertices = mesh.vertices
    polygons = mesh.polygons

    # If not triangulated!
    print("\n")
    if len(polygons[0].vertices) != 3:
        print("len(polygons[0].vertices) != 3, triangulating mesh....")
        triangulate(active_object)
   
    # @debug info 
    print("Active name:", active_object.name)
    print("Vertexcount: ", len(vertices))
    print("TriangleCount: ", len(polygons))      
    print("Outputfile:", active_object.name+".yml")

    outfile = open(active_object.name+".yml", "w")
    
    writeVertices(outfile, vertices)
    outfile.write("\n")

    outfile.write("meshes: 1\n");
    outfile.write("mesh: blendermesh\n")
    outfile.write("material: default\n")
    outfile.write("shader: default\n")
    writeTriangles(outfile, polygons)
        
    outfile.close()

