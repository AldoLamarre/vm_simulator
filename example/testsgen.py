from random import randint


taille = 256*256

def create_test(title, message, nb_instruction):
    i=0
    while(i<nb_instruction and len(message) > 0 ):
        rw = randint(0,1)
        if(rw):
            create_test_read(title)
        else:            
            message = create_test_write(title,message)
        i+=1            
        
def create_test_read(title):
    global taille
    with open(title+".in","a") as fd:
        r=randint(0,taille)        
        fd.write("r"+ str(r) +";\n")        
            
def create_test_write(title,message):
    global taille 
    with open(title+".in","a") as fd:    
        i=0
        length=len(message)      
        msg=list(message)
        i = randint(0, taille)
        fd.write("w"+ str(i) +"'"+message[0]+"';\n")          
    return message[1:]
def random_message(length):
    message = ""
    i=0
    while(i<length):
        randc = randint(65,90)
        message += chr(randc)
        i+=1
    return message        
            
if __name__ == "__main__":   
    create_test("testio5", random_message(65536*2) , 65536*6)
    
        
        
        
        