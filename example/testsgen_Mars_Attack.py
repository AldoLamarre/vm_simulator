with open("testpage2.in","w") as fd:
    i=0
    x=0
    msg = "ack ack ack mars attack "
    msg=list(msg)
    while(i<(256*256)):        
        fd.write("w"+ str(i) +"'"+msg[i % len(msg)]+"';\n")
        i+=1
    
        
        
        
        