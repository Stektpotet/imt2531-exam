import os
import bpy

def writeModel(outstring):
    cwd = os.getcwd()

    filepath = "out.yml"
    with open(filepath, "w") as file:
        print("Writing to: ", str(cwd) + filepath)
        file.write(str(cwd))

def writeModelWithoutUVandColor(vcontainer):
    outstring = ""
    
    print("vertices:", len(vcontainer))
    for vc in vcontainer:
        print("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}".format( 
            vc.co[0],
            vc.co[1],
            vc.co[2],
            vc.normal[0],
            vc.normal[1],
            vc.normal[2],
            0, 0, 255, 255, 255, 255))
            
    return




mesh = bpy.data.objects["Cube"].data
print("Mesh: ", mesh)

vertexContainer = mesh.vertices 
print("Vertices: ", vertexContainer)

hasUV = mesh.uv_layers.active
print("HasUV: ", hasUV)


if not hasUV:
    print("HashUV is None")
    writeModelWithoutUVandColor(vertexContainer)

vertices = []
normals  = []
for v in vertexContainer:
    vertices.append(v.co)
    normals.append(v.normal)

polygons = mesh.polygons
print("Polygons: ", polygons)



