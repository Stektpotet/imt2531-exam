import os
import bpy

# input
out = ""
scale = .02
meshtag = "Top"
outfile = open("top.yml", "w")

def writeVertexWithoutUVandColor(outfile, vcontainer):
    global scale
    outfile.write("vertices: {}\n".format(len(vcontainer)))
    for vc in vcontainer:
        outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}\n".format( 
                vc.co[0]*scale,
                (vc.co[1]*scale)-0.5,
                vc.co[2]*scale,
                vc.normal[0],
                vc.normal[1],
                vc.normal[2],
                0, 0, 255, 255, 255, 255))
                


def writeTriangles(outfile, polygons):
    
    outfile.write("triangles: {}\n".format(len(polygons)))
    for p in polygons:
        if len(p.vertices) != 3:
            print("ERROR, TRIANGULATE YOUR MESH!!!!")
            exit(-1)
            
        outfile.write("t: {} {} {}\n".format(
            p.vertices[0],
            p.vertices[1],
            p.vertices[2]
        ))


# processing
mesh = bpy.data.objects[meshtag].data
print("Mesh: ", mesh)

vertexContainer = mesh.vertices 
print("Vertices: ", vertexContainer)

hasUV = mesh.uv_layers.active
print("HasUV: ", hasUV)

polygons = mesh.polygons
print("Polygons: ", polygons)



print("HashUV is None")
    
writeVertexWithoutUVandColor(outfile, vertexContainer)
outfile.write("\n")

# iterate all meshes
outfile.write("meshes: 1\n")
outfile.write("mesh: " + meshtag + "\n")
outfile.write("material: default\n")
outfile.write("shader: default\n")
    
writeTriangles(outfile, polygons)
outfile.write("\n")
    
outfile.close()

