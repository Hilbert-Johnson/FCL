center = [0,0,0]
scale = 0.14

def write_v(f, v):
    f.write("v {:.5f} {:.5f} {:.5f}\n".format(v[0]*scale+center[0], v[1]*scale+center[1], v[2]*scale+center[2]))

def write_t(f, t):
    f.write("f "+str(t[0])+" "+str(t[2])+" "+str(t[1])+"\n")

with open('tie.obj', 'w') as f:
    write_v(f, [0.0, 0.0, 0.0])
    for i in range(20):
        y = 5+60/20*i
        x = 5*(y-5)/(60*2)
        write_v(f, [-5+x, 0.0, y])
        write_v(f, [0.0, 0.0, y])
        write_v(f, [5-x, 0.0, y])
    for j in range(31):
        y = 65+90/30*j
        write_v(f, [-2.5, 0.0, y])
        write_v(f, [0.0, 0.0, y])
        write_v(f, [2.5, 0.0, y])
    write_v(f, [0.0, 0.0, 157.5])
    write_t(f, [1, 3, 2])
    write_t(f, [1, 4, 3])
    for i in range(50):
        write_t(f, [2+3*i, 3+3*i, 6+3*i])
        write_t(f, [3+3*i, 4+3*i, 6+3*i])
        write_t(f, [2+3*i, 6+3*i, 5+3*i])
        write_t(f, [4+3*i, 7+3*i, 6+3*i])
    write_t(f, [152, 153, 155])
    write_t(f, [153, 154, 155])