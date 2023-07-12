import unittest
import pyCAPS
import ESPtools
import exodus

csmFileName = "dummy_despmtrs.csm"
def writeCsmFile(contents):
    csmFile = open(csmFileName, "w")
    csmFile.writelines(contents)
    csmFile.close()

def constructCAPSProblem(csmFilename):
    return pyCAPS.Problem(problemName = "ESP_Scratch_Dir", capsFile=csmFileName)

class getInitialValues(unittest.TestCase):
    def test_errorNoDespmtrKeyword(self):
        writeCsmFile([
            "designparameter Lx 12.4"
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: no keyword 'despmtr' found in file 'dummy_despmtrs.csm'")

    def test_errorMoreThanNineTokens(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 initial 2.5 final 45.6"
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorFewerThanThreeTokens(self):
        writeCsmFile([
            "despmtr Lx"
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorBetweenThreeAndNineTokens(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 initial 2.5"
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorInitialNotFoundAsTokenWhereExpected(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 starting 2.5 \n"
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: expected 'initial' token, got 'starting'")

    def test_getCorrectValuesWithOnlyDespmtrKeyword(self):
        writeCsmFile([
            "despmtr Lx 0.523 \n",
            "despmtr Ly 13.2"
        ])

        params = ESPtools.getInitialValues(csmFileName)

        self.assertEqual(len(params), 2)
        self.assertEqual(params[0], 0.523)
        self.assertEqual(params[1], 13.2)

    def test_getCorrectValuesWithAllKeywords(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 initial 2.5 \n",
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n",
            "despmtr Lz 0.09834 lbound 5.0 ubound 8.0 initial 6.5"
        ])

        params = ESPtools.getInitialValues(csmFileName)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 2.5)
        self.assertEqual(params[1], 3.5)
        self.assertEqual(params[2], 6.5)

    def test_getCorrectValuesWithMixedKeywords(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n",
            "despmtr Lz 0.09834"
        ])

        params = ESPtools.getInitialValues(csmFileName)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 2.5)
        self.assertEqual(params[1], 3.5)
        self.assertEqual(params[2], 0.09834)

class getCurrentValues(unittest.TestCase):
    def test_errorNoDespmtrsInCAPSProblem(self):
        writeCsmFile([
            "conpmtr Lx 2.5 \n",
            "conpmtr Ly 39.5 \n",
            "conpmtr Lz 0.09834"
        ])
        problem = constructCAPSProblem(csmFileName)

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getCurrentValues(problem)

        self.assertEqual(str(errMsg.exception), "Error: no 'despmtr' objects in the CAPS problem constructed with the given csm file.")

    def test_getCorrectValuesWithAllKeywords(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 initial 2.5 \n",
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n",
            "despmtr Lz 0.09834 lbound 5.0 ubound 8.0 initial 6.5"
        ])
        problem = constructCAPSProblem(csmFileName)

        params = ESPtools.getCurrentValues(problem)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 2.5)
        self.assertEqual(params[1], 39.5)
        self.assertEqual(params[2], 0.09834)

    def test_getCorrectValuesWithOnlyDespmtrKeyword(self):
        writeCsmFile([
            "despmtr Lx 0.523 \n",
            "despmtr Ly 13.2"
        ])
        problem = constructCAPSProblem(csmFileName)

        params = ESPtools.getCurrentValues(problem)

        self.assertEqual(len(params), 2)
        self.assertEqual(params[0], 0.523)
        self.assertEqual(params[1], 13.2)

    def test_getCorrectValuesWithMixedKeywords(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n",
            "despmtr Lz 4.5 lbound 3.0 ubound 5.0 initial 3.5"
        ])
        problem = constructCAPSProblem(csmFileName)

        params = ESPtools.getCurrentValues(problem)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 2.5)
        self.assertEqual(params[1], 39.5)
        self.assertEqual(params[2], 4.5)

class setDesignParameterValues(unittest.TestCase):
    def test_errorFewerDespmtrsThanExpected(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Lz 0.09834"
        ])
        problem = constructCAPSProblem(csmFileName)

        newVals = [10.0, 9.0, 8.0]
        with self.assertRaises(Exception) as errMsg:
            ESPtools.setDesignParameterValues(problem, newVals)

        self.assertEqual(str(errMsg.exception), "Error: Number of values provided does not equal the number of 'despmtr' objects in CAPS problem.")

    def test_dummyValuesSetCorrectly(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 \n",
            "despmtr Lz 0.09834"
        ])
        problem = constructCAPSProblem(csmFileName)

        newVals = [10.0, 9.0, 8.0]
        ESPtools.setDesignParameterValues(problem, newVals)

        params = ESPtools.getCurrentValues(problem)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 10.0)
        self.assertEqual(params[1], 9.0)
        self.assertEqual(params[2], 8.0)

class parametersAreEqual(unittest.TestCase):
    def test_parametersHaveDifferentLengths(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 \n",
            "despmtr Lz 0.09"
        ])
        problem = constructCAPSProblem(csmFileName)

        initialParams = ESPtools.getInitialValues(csmFileName)

        newVals = [2.5, 39.5, 0.09, 13.4]

        paramsAreSame = ESPtools.parametersAreEqual(initialParams, newVals)
        self.assertFalse(paramsAreSame)

    def test_parametersAreNotEqual(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 \n",
            "despmtr Lz 0.09"
        ])
        problem = constructCAPSProblem(csmFileName)

        initialParams = ESPtools.getInitialValues(csmFileName)

        newVals = [10.0, 9.0, 8.0]
        ESPtools.setDesignParameterValues(problem, newVals)
        newParams = ESPtools.getCurrentValues(problem)

        paramsAreSame = ESPtools.parametersAreEqual(initialParams, newParams)
        self.assertFalse(paramsAreSame)

    def test_parametersAreNotEqualWithinTolerance(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 \n",
            "despmtr Lz 0.09"
        ])
        problem = constructCAPSProblem(csmFileName)

        initialParams = ESPtools.getInitialValues(csmFileName)

        newVals = [2.5, 39.5000001, 0.09]
        ESPtools.setDesignParameterValues(problem, newVals)
        newParams = ESPtools.getCurrentValues(problem)

        paramsAreSame = ESPtools.parametersAreEqual(initialParams, newParams)
        self.assertFalse(paramsAreSame)

    def test_parametersAreEqual(self):
        writeCsmFile([
            "despmtr Lx 2.5 \n",
            "despmtr Ly 39.5 \n",
            "despmtr Lz 0.09"
        ])
        problem = constructCAPSProblem(csmFileName)

        initialParams = ESPtools.getInitialValues(csmFileName)

        newVals = [2.5, 39.5, 0.09]
        ESPtools.setDesignParameterValues(problem, newVals)
        newParams = ESPtools.getCurrentValues(problem)

        paramsAreSame = ESPtools.parametersAreEqual(initialParams, newParams)
        self.assertTrue(paramsAreSame)

class Aflr4Aflr3Meshing(unittest.TestCase):
    def writeBoxMeshCsm(self, Lx, Ly, Lz):
        writeCsmFile([
            "attribute capsAIM $aflr4AIM;aflr3AIM;platoAIM \n",
            "attribute capsMeshLength 5.0 \n",
            "despmtr Lx " + str(Lx) + " lbound 0.5 ubound 3.0 initial 1.0 \n",
            "despmtr Ly " + str(Ly) + " lbound 1.0 ubound 5.0 initial 2.0 \n",
            "despmtr Lz " + str(Lz) + " lbound 1.0 ubound 8.0 initial 1.5 \n",
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

    def test_morphedMeshConnectivityMatchesInitialMesh(self):
        etoName = "dummy_eto.eto"

        self.writeBoxMeshCsm(Lx=1.0, Ly=2.0, Lz=1.5)
        initialMeshName = "initial_mesh.exo"
        ESPtools.aflr4_aflr3_meshing(csmFileName, initialMeshName, 0.2, 1.0, 1.0, etoName, meshMorph=False)
        initialMesh = exodus.ExodusDB()
        initialMesh.read(initialMeshName)

        self.writeBoxMeshCsm(Lx=1.1, Ly=2.0, Lz=1.5)
        perturbedMeshName = "perturbed_mesh.exo"
        ESPtools.aflr4_aflr3_meshing(csmFileName, perturbedMeshName, 0.2, 1.0, 1.0, etoName, meshMorph=False)
        perturbedMesh = exodus.ExodusDB()
        perturbedMesh.read(perturbedMeshName)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)



if __name__ == '__main__':
    unittest.main()
