import unittest
import ESPtools

csmFileName = "dummy_despmtrs.csm"
def writeCsmFile(contents):
    csmFile = open(csmFileName, "w")
    csmFile.writelines(contents)
    csmFile.close()

class getInitialValues(unittest.TestCase):
    def test_errorNoDespmtrKeyword(self):
        writeCsmFile([
            "designparameter Lx 12.4",
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: no keyword 'despmtr' found in file 'dummy_despmtrs.csm'")

    def test_errorMoreThanNineTokens(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 initial 2.5 final 45.6",
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorFewerThanThreeTokens(self):
        writeCsmFile([
            "despmtr Lx",
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorBetweenThreeAndNineTokens(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 initial 2.5",
        ])

        with self.assertRaises(Exception) as errMsg:
            ESPtools.getInitialValues(csmFileName)

        self.assertEqual(str(errMsg.exception), "Parsing error: reading initial values failed.")

    def test_errorInitialNotFoundAsTokenWhereExpected(self):
        writeCsmFile([
            "despmtr Lx 2.5 lbound 2.0 ubound 3.0 starting 2.5 \n",
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
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n"
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
            "despmtr Ly 39.5 lbound 3.0 ubound 5.0 initial 3.5 \n"
            "despmtr Lz 0.09834"
        ])

        params = ESPtools.getInitialValues(csmFileName)

        self.assertEqual(len(params), 3)
        self.assertEqual(params[0], 2.5)
        self.assertEqual(params[1], 3.5)
        self.assertEqual(params[2], 0.09834)

if __name__ == '__main__':
    unittest.main()
