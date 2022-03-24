/*
 * XMLGeneratorFlaskUtilities.cpp
 *
 *  Created on: March 24, 2022
 */

#include <fstream>

#include "XMLGeneratorFlaskUtilities.hpp"
#include "XMLGeneratorOperationsDataStructures.hpp"

namespace XMLGen
{

namespace flask
{

void write_web_app_executable
(const std::string& aBaseFileName,
 const std::string& aFileExtension,
 XMLGen::OperationMetaData& aOperationMetaData)
{
    for(auto& tWebPortNum : aOperationMetaData.get("web_port_numbers"))
    {
        auto tIndex = &tWebPortNum - &aOperationMetaData.get("web_port_numbers")[0];
        auto tFileName = aBaseFileName + "_" + std::to_string(tIndex) + aFileExtension;
        aOperationMetaData.append("run_app_files", tFileName);

        std::ofstream tOutFile;
        tOutFile.open(tFileName, std::ofstream::out | std::ofstream::trunc);
        tOutFile << "from flask import Flask\n";
        tOutFile << "import subprocess\n";
        tOutFile << "app = Flask(__name__)\n\n";
    
        tOutFile << "@app.route(\'" << "/run_" + aOperationMetaData.get("executables")[tIndex] + "/\')\n";
        tOutFile << "def run_gemma():\n";
        tOutFile << "    tExitStatus = subprocess.call([";
        for(auto& tArgument : aOperationMetaData.get("arguments"))
        {
            auto tIndex = &tArgument - &aOperationMetaData.get("arguments")[0];
            auto tDelimiter = tIndex !=  (aOperationMetaData.get("arguments").size() - 1u) ? ", " : "";
            tOutFile << "\"" << tArgument << "\"" << tDelimiter;
        }
        tOutFile << "])\n";
        tOutFile << "    return str(tExitStatus)\n";
        tOutFile << "if __name__ == \'__main__\':\n";
        tOutFile << "    app.run(host=\'0.0.0.0\', port=" << tWebPortNum << ")";
    
        tOutFile.close();
    }
}
// function write_web_app_executable

}
// namespace flask

}
// namespace XMLGen