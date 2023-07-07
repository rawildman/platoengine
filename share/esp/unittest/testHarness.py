import unittest
import pyCAPS
import ESPtools

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

if __name__ == '__main__':
    unittest.main()
