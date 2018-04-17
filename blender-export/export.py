import os
import bpy

def appendline(dest, src):
    return dest + src + "\n"

def writeModelfile(filepath, outstring):
    cwd = os.getcwd()
    with open(filepath, "w") as file:
        print("Writing to: ", str(cwd) + filepath)
        file.write(outstring)


def writeVertexWithoutUVandColor(vcontainer):
    out = ""
    
    out = appendline(out, "vertices:".format(len(vcontainer)))
    for vc in vcontainer:
        out = appendline(out, "v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}".format( 
            vc.co[0],
            vc.co[1],
            vc.co[2],
            vc.normal[0],
            vc.normal[1],
            vc.normal[2],
            0, 0, 255, 255, 255, 255))
                
    return out


def writeTriangles(polygons):
    out = ""
    
    out = appendline(out, "meshes: 1")
    out = appendline(out, "mesh: Cube")
    out = appendline(out, "  material: base")
    out = appendline(out, "  shader: base")
    out = appendline(out, "  triangles: {}".format(len(polygons)))

    for p in polygons:
        if len(p.vertices) != 3:
            print("ERROR, TRIANGULATE YOUR MESH!!!!")
            exit(-1)
            
        out = appendline(out, "  t: {} {} {}".format(
            p.vertices[0],
            p.vertices[1],
            p.vertices[2]
        ))
    return out


# input
out = ""
meshtag = "Cube"

# processing
mesh = bpy.data.objects[meshtag].data
print("Mesh: ", mesh)

vertexContainer = mesh.vertices 
print("Vertices: ", vertexContainer)

hasUV = mesh.uv_layers.active
print("HasUV: ", hasUV)

polygons = mesh.polygons
print("Polygons: ", polygons)


if not hasUV:
    print("HashUV is None")
    out = appendline(out, writeVertexWithoutUVandColor(vertexContainer))
    out = appendline(out, writeTriangles(polygons))

    
vertices = []
normals  = []
for v in vertexContainer:
    vertices.append(v.co)
    normals.append(v.normal)


print(out)
writeModelfile("out.yml", out)