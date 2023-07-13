import unittest
import pyCAPS
import ESPtools
import exodus
import csmFileGenerator

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

class MeshMorphInAflr4Aflr3Workflow(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(MeshMorphInAflr4Aflr3Workflow, self).__init__(*args, **kwargs)
        self.writer = csmFileGenerator.csmFileGenerator(csmFileName)

    def generateMesh(self, csmName, meshName, meshMorph):
        ESPtools.aflr4_aflr3_meshing(csmName, meshName, meshMorph)
        mesh = exodus.ExodusDB()
        mesh.read(meshName)
        return mesh

    def test_BoxConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeBoxMeshCsm(Lx=1.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeBoxMeshCsm(Lx=1.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_BoxConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeBoxMeshCsm(Lx=1.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeBoxMeshCsm(Lx=1.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_SphereConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeSphereMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeSphereMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_SphereConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeSphereMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeSphereMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_CylinderConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeCylinderMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeCylinderMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_CylinderConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeCylinderMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeCylinderMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_ConeConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeConeMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeConeMeshCsm(radius=2.2)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_ConeConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeConeMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeConeMeshCsm(radius=2.2)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_ExtrudedCubeConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeExtrudedCubeMeshCsm(length=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeExtrudedCubeMeshCsm(length=2.5)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_ExtrudedCubeConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeExtrudedCubeMeshCsm(length=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeExtrudedCubeMeshCsm(length=2.5)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_CubeWithCylindricalHoleConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writeCubeWithCylindricalHoleMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeCubeWithCylindricalHoleMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_CubeWithCylindricalHoleConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writeCubeWithCylindricalHoleMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writeCubeWithCylindricalHoleMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_PlusSignConnectivityDoesNotMatchInitialWhenRemeshed(self):
        self.writer.writePlusSignMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writePlusSignMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=False)

        self.assertNotEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertNotEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertNotEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)

    def test_PlusSignConnectivityMatchesInitialWhenMorphed(self):
        self.writer.writePlusSignMeshCsm(radius=2.0)
        initialMeshName = "initial_mesh.exo"
        initialMesh = self.generateMesh(csmFileName, initialMeshName, meshMorph=False)

        self.writer.writePlusSignMeshCsm(radius=2.1)
        perturbedMeshName = "perturbed_mesh.exo"
        perturbedMesh = self.generateMesh(csmFileName, perturbedMeshName, meshMorph=True)

        self.assertEqual(initialMesh.numNodes, perturbedMesh.numNodes)
        self.assertEqual(initialMesh.numElements, perturbedMesh.numElements)
        self.assertEqual(initialMesh.elementBlocks[0].connectivity, perturbedMesh.elementBlocks[0].connectivity)



if __name__ == '__main__':
    unittest.main()
