import unittest
import ESPtools
import exodus
import csmFileGenerator

csmFileName = "dummy_despmtrs.csm"
writer = csmFileGenerator.csmFileGenerator(csmFileName)

def generateMesh(csmFunc, val, meshName, meshMorph):
    csmFunc(val)
    ESPtools.aflr4_aflr3_meshing(csmFileName, meshName, meshMorph, quiet=True)
    mesh = exodus.ExodusDB()
    mesh.read(meshName)
    return mesh

def generateInitialMesh(csmFunc, val):
    initialMeshName = "initial_mesh.exo"
    mesh = generateMesh(csmFunc, val, initialMeshName, meshMorph=False)
    return mesh

def generatePerturbedMesh(csmFunc, val, meshMorph):
    initialMeshName = "perturbed_mesh.exo"
    mesh = generateMesh(csmFunc, val, initialMeshName, meshMorph=meshMorph)
    return mesh

class MeshMorphInAflr4Aflr3Workflow(unittest.TestCase):
    def assertMeshesAreDifferent(self, mesh1, mesh2):
        self.assertNotEqual(mesh1.numNodes, mesh2.numNodes)
        self.assertNotEqual(mesh1.numElements, mesh2.numElements)
        self.assertNotEqual(mesh1.elementBlocks[0].connectivity, mesh2.elementBlocks[0].connectivity)

    def assertMeshesAreSame(self, mesh1, mesh2):
        self.assertEqual(mesh1.numNodes, mesh2.numNodes)
        self.assertEqual(mesh1.numElements, mesh2.numElements)
        self.assertEqual(mesh1.elementBlocks[0].connectivity, mesh2.elementBlocks[0].connectivity)

    def test_BoxConnectivityDoesNotMatchInitialWhenRemeshed(self):
        Lx = 1.0
        initialMesh = generateInitialMesh(writer.writeBoxMeshCsm, Lx)

        Lx = 1.1
        perturbedMesh = generatePerturbedMesh(writer.writeBoxMeshCsm, Lx, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_BoxConnectivityMatchesInitialWhenMorphed(self):
        Lx = 1.0
        initialMesh = generateInitialMesh(writer.writeBoxMeshCsm, Lx)

        Lx = 1.1
        perturbedMesh = generatePerturbedMesh(writer.writeBoxMeshCsm, Lx, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_SphereConnectivityDoesNotMatchInitialWhenRemeshed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeSphereMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeSphereMeshCsm, radius, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_SphereConnectivityMatchesInitialWhenMorphed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeSphereMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeSphereMeshCsm, radius, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_CylinderConnectivityDoesNotMatchInitialWhenRemeshed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeCylinderMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeCylinderMeshCsm, radius, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_CylinderConnectivityMatchesInitialWhenMorphed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeCylinderMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeCylinderMeshCsm, radius, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_ConeConnectivityDoesNotMatchInitialWhenRemeshed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeConeMeshCsm, radius)

        radius = 2.2
        perturbedMesh = generatePerturbedMesh(writer.writeConeMeshCsm, radius, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_ConeConnectivityMatchesInitialWhenMorphed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeConeMeshCsm, radius)

        radius = 2.2
        perturbedMesh = generatePerturbedMesh(writer.writeConeMeshCsm, radius, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_ExtrudedCubeConnectivityDoesNotMatchInitialWhenRemeshed(self):
        length = 2.0
        initialMesh = generateInitialMesh(writer.writeExtrudedCubeMeshCsm, length)

        length = 2.5
        perturbedMesh = generatePerturbedMesh(writer.writeExtrudedCubeMeshCsm, length, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_ExtrudedCubeConnectivityMatchesInitialWhenMorphed(self):
        length = 2.0
        initialMesh = generateInitialMesh(writer.writeExtrudedCubeMeshCsm, length)

        length = 2.5
        perturbedMesh = generatePerturbedMesh(writer.writeExtrudedCubeMeshCsm, length, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_CubeWithCylindricalHoleConnectivityDoesNotMatchInitialWhenRemeshed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeCubeWithCylindricalHoleMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeCubeWithCylindricalHoleMeshCsm, radius, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_CubeWithCylindricalHoleConnectivityMatchesInitialWhenMorphed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writeCubeWithCylindricalHoleMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writeCubeWithCylindricalHoleMeshCsm, radius, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)

    def test_PlusSignConnectivityDoesNotMatchInitialWhenRemeshed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writePlusSignMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writePlusSignMeshCsm, radius, meshMorph=False)

        self.assertMeshesAreDifferent(initialMesh, perturbedMesh)

    def test_PlusSignConnectivityMatchesInitialWhenMorphed(self):
        radius = 2.0
        initialMesh = generateInitialMesh(writer.writePlusSignMeshCsm, radius)

        radius = 2.1
        perturbedMesh = generatePerturbedMesh(writer.writePlusSignMeshCsm, radius, meshMorph=True)

        self.assertMeshesAreSame(initialMesh, perturbedMesh)



if __name__ == '__main__':
    unittest.main()
