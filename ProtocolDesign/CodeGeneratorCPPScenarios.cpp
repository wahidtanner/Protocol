//
//  CodeGeneratorCPPScenarios.cpp
//  Protocol
//
//  Created by Wahid Tanner on 10/18/14.
//

#include <string>

#include "../Submodules/Designer/Designer/Designer.h"

#include "../Protocol/CodeGeneratorManager.h"
#include "../Protocol/ProtoParser.h"

using namespace std;
using namespace MuddledManaged;

DESIGNER_SCENARIO( CodeGeneratorCPP, "Operation/Normal", "CPP generator can create empty file." )
{
    Protocol::CodeGeneratorManager * pManager = Protocol::CodeGeneratorManager::instance();

    auto generator = pManager->generator("CPlusPlus");

    Protocol::ProtoParser parser("Empty.proto");
    auto model = parser.parse();

    generator->generateCode("Generated", *model);
}

DESIGNER_SCENARIO( CodeGeneratorCPP, "Operation/Normal", "CPP generator can create file with enums." )
{
    Protocol::CodeGeneratorManager * pManager = Protocol::CodeGeneratorManager::instance();

    auto generator = pManager->generator("CPlusPlus");

    Protocol::ProtoParser parser("EnumMultiple.proto");
    auto model = parser.parse();

    generator->generateCode("Generated", *model);
}

DESIGNER_SCENARIO( CodeGeneratorCPP, "Operation/Normal", "CPP generator can create file with enum and values." )
{
    Protocol::CodeGeneratorManager * pManager = Protocol::CodeGeneratorManager::instance();

    auto generator = pManager->generator("CPlusPlus");

    Protocol::ProtoParser parser("EnumValue.proto");
    auto model = parser.parse();

    generator->generateCode("Generated", *model, "ProtocolDesign");
}

DESIGNER_SCENARIO( CodeGeneratorCPP, "Operation/Normal", "CPP generator can create file with imports, message, and enum." )
{
    Protocol::CodeGeneratorManager * pManager = Protocol::CodeGeneratorManager::instance();

    auto generator = pManager->generator("CPlusPlus");

    Protocol::ProtoParser parser("ImportPublic.proto");
    auto model = parser.parse();

    generator->generateCode("Generated", *model, "ProtocolDesign");
}

DESIGNER_SCENARIO( CodeGeneratorCPP, "Operation/Normal", "CPP generator can create file with message fields." )
{
    Protocol::CodeGeneratorManager * pManager = Protocol::CodeGeneratorManager::instance();

    auto generator = pManager->generator("CPlusPlus");

    Protocol::ProtoParser parser("MessageField.proto");
    auto model = parser.parse();

    generator->generateCode("Generated", *model, "ProtocolDesign");
}
