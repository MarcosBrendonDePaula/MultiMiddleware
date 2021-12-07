import os,sys
# Fuctions
def getAll(path="",exp=".cpp"):
    files = []
    for diretorio, subpastas, arquivos in os.walk(path):
        for arquivo in arquivos:
            arq_path = os.path.join(diretorio, arquivo)
            if(arq_path.find(exp) >=0):
                files.append(arq_path.replace("\\","/"))
    return files

def ListToStr(list = []):
    string = ""
    for i in list:
        string+=i+" "
    return string


# CONFIGS
Compiller = "g++"
output = ""
include_path   = "include"
src_path       = "src"


flags = [
    "-O3",
    "-std=c++17"
]


link_libraries = [
    "ws2_32"
]

cpp_files = getAll(include_path,".cpp")
cpp_src_Server = getAll(src_path+"/Server",".cpp")
cpp_src_Client = getAll(src_path+"/Client",".cpp")

if len(sys.argv) < 2:
    print("-server      Compilar Servidor")
    print("-client      Compilar Cliente")
    print("-all         Compilar Servidor e cliente")
else:
    for param in sys.argv:
        if(param.upper() == "-Server".upper()):
            print("Compilando Servidor")
            os.system("{comp} {flags} {includes} {src} -o Server -l {libraries} -I {include_path}".format(
                comp = Compiller,
                flags = ListToStr(flags),  
                includes = ListToStr(cpp_files),
                src = ListToStr(cpp_src_Server),
                out = output,
                libraries = ListToStr(link_libraries),
                include_path = include_path
            ))
            print("Servidor Pronto")
        elif(param.upper() == "-Client".upper()) :
            print("Compilando Client")
            os.system("{comp} {flags} {includes} {src} -o Client -l {libraries} -I {include_path}".format(
                comp = Compiller,  
                flags = ListToStr(flags),
                includes = ListToStr(cpp_files),
                src = ListToStr(cpp_src_Client),
                out = output,
                libraries = ListToStr(link_libraries),
                include_path = include_path
            ))
            print("Cliente Pronto")
        elif(param.upper() == "-all".upper()):
            print("Compilando Servidor")
            os.system("{comp} {flags} {includes} {src} -o Server -l {libraries} -I {include_path}".format(
                comp = Compiller,
                flags = ListToStr(flags),  
                includes = ListToStr(cpp_files),
                src = ListToStr(cpp_src_Server),
                out = output,
                libraries = ListToStr(link_libraries),
                include_path = include_path
            ))
            print("Servidor Pronto")
            print("Compilando Client")
            os.system("{comp} {flags} {includes} {src} -o Client -l {libraries} -I {include_path}".format(
                comp = Compiller,  
                flags = ListToStr(flags),
                includes = ListToStr(cpp_files),
                src = ListToStr(cpp_src_Client),
                out = output,
                libraries = ListToStr(link_libraries),
                include_path = include_path
            ))
            print("Cliente Pronto")
    pass




