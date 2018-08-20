# Project:   mpglib


# Toolflags:
c++flags =  -c -depend !Depend -throwback  +p -faf  -Ospace   -apcs 3/32/fpe2/swst/fp/nofpr -IC:,ADFS::Tardis.$.Files.Develop.Projects.Libraries.lame 
ccflags =  -c -depend !Depend -throwback   -fafh    -Ospace  -apcs 3/32/fpe2/swst/fp/nofpr  -DRISCOS -DHAVE_CONFIG_H -IC:,ADFS::Tardis.$.Files.Develop.Projects.Libraries.lame 
gccflags =  -c -depend       -I C: -I ADFS::Tardis.$.Files.Develop.Projects.Libraries.lame  
objasmflags =  -Depend !Depend -Throwback   -32   -I C:,ADFS::Tardis.$.Files.Develop.Projects.Libraries.lame 
findflags =  -t     -u  -f  @.c.* @.h.* 
libfileflags =  -o  -c   
extractsymflags =  o 
rinkflags =   -o seg -l lnk   -a 
rinkptrflags = rinkptr rblk. o.
linkflags =               
cmhgflags =     -depend !Depend   
cmungeflags =  $(cmhg)   -depend !Depend  -I $includefile 
squeezeflags =    

# Final targets:
@.mpglib:   @.o.common @.o.dct64_i386 @.o.decode_i386 @.o.interface @.o.layer1 @.o.layer2 @.o.layer3 @.o.tabinit 
        libfile $(libfileflags) @.o.common \
        @.o.dct64_i386 @.o.decode_i386 @.o.interface @.o.layer1 @.o.layer2 @.o.layer3 @.o.tabinit 

# User-editable dependencies:

# Static dependencies:
@.o.common:   @.c.common
        cc $(ccflags) -o @.o.common @.c.common
@.o.dct64_i386:   @.c.dct64_i386
        cc $(ccflags) -o @.o.dct64_i386 @.c.dct64_i386
@.o.decode_i386:   @.c.decode_i386
        cc $(ccflags) -o @.o.decode_i386 @.c.decode_i386
@.o.interface:   @.c.interface
        cc $(ccflags) -o @.o.interface @.c.interface
@.o.layer1:   @.c.layer1
        cc $(ccflags) -o @.o.layer1 @.c.layer1
@.o.layer2:   @.c.layer2
        cc $(ccflags) -o @.o.layer2 @.c.layer2
@.o.layer3:   @.c.layer3
        cc $(ccflags) -o @.o.layer3 @.c.layer3
@.o.tabinit:   @.c.tabinit
        cc $(ccflags) -o @.o.tabinit @.c.tabinit

# Dynamic dependencies:
