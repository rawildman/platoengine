class csmFileGenerator:
    def __init__(self, csmFileName):
        self.csmFileName = csmFileName
    
    def writeCsmFile(self, contents):
        csmFile = open(self.csmFileName, "w")
        csmFile.writelines(contents)
        csmFile.close()

    def writeBoxMeshCsm(self, Lx):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr Lx " + str(Lx) + " lbound 0.5 ubound 3.0 initial 1.0 \n",
            "conpmtr Ly 2.0 \n",
            "conpmtr Lz 1.5 \n",
            "box -Lx/2   -Ly/2   -Lz/2   Lx   Ly   Lz \n",
            "select body 1 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writeSphereMeshCsm(self, radius):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr radius " + str(radius) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "sphere 0   0   0   radius \n",
            "select body 1 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writeCylinderMeshCsm(self, radius):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr radius " + str(radius) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "cylinder 0   0   0   2   0   0   radius \n",
            "select body 1 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writeConeMeshCsm(self, radius):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr radius " + str(radius) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "cone 4   0   0   1   0   0   radius \n",
            "select body 1 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writeExtrudedCubeMeshCsm(self, length):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr length " + str(length) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "skbeg   -length/2   -length/2   0 \n",
	            "linseg   -length/2   length/2   0 \n",
	            "linseg   length/2   length/2   0 \n",
	            "linseg   length/2   -length/2   0 \n",
	            "linseg   -length/2   -length/2   0 \n",
            "skend     0 \n",
            "extrude 0 0 length \n",
            "select body 7 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writeCubeWithCylindricalHoleMeshCsm(self, radius):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 10.0 \n",
            "conpmtr length 7.0 \n",
            "despmtr radius " + str(radius) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "box -length/2   -length/2   -length/2   length   length   length \n",
            "cylinder 0   0   -length/2   0   0   length/2   radius \n",
            "subtract \n",
            "select body 3 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])

    def writePlusSignMeshCsm(self, radius):
        self.writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 20.0 \n",
            "conpmtr length 5.0 \n",
            "despmtr radius " + str(radius) + " lbound 1.0 ubound 3.0 initial 2.0 \n",
            "cylinder 0   0   -length   0   0   length   radius \n",
            "cylinder -length   0   0   length   0   0   radius \n",
            "union \n",
            "select body 3 \n",
            "select face \n",
            "attribute capsGroup   $solid_group \n",
            "patbeg i @stack.size \n",
            "  select body @stack[i] \n", 
            "  attribute _name $block_+val2str(i,0) \n",
            "patend \n",
            "end"
        ])