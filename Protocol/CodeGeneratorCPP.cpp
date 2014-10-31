//
//  CodeGeneratorCPP.cpp
//  Protocol
//
//  Created by Wahid Tanner on 10/17/14.
//

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>

#include "CodeGeneratorCPP.h"

using namespace std;
using namespace boost;
using namespace MuddledManaged;

const string Protocol::CodeGeneratorCPP::mHeaderFileExtension = ".protocol.h";
const string Protocol::CodeGeneratorCPP::mSourceFileExtension = ".protocol.cpp";
const string Protocol::CodeGeneratorCPP::mHeaderFileProlog =
"// This file was generated from the Protocol compiler.\n"
"// You should not edit this file directly.\n";
const string Protocol::CodeGeneratorCPP::mSourceFileProlog =
"// This file was generated from the Protocol compiler.\n"
"// You should not edit this file directly.\n";

Protocol::CodeGeneratorCPP::CodeGeneratorCPP ()
{ }

void Protocol::CodeGeneratorCPP::generateCode (const string & outputFolder, const ProtoModel & model, const std::string & projectName) const
{
    filesystem::path outputPath(outputFolder);
    filesystem::path modelPath(model.fileName());
    filesystem::path headerPath(outputPath / filesystem::change_extension(modelPath, mHeaderFileExtension));
    filesystem::path sourcePath(outputPath / filesystem::change_extension(modelPath, mSourceFileExtension));

    filesystem::create_directory(outputFolder);
    filesystem::ofstream headerFile(headerPath, ios::out | ios::trunc);
    CodeWriter headerFileWriter(headerFile);
    filesystem::ofstream sourceFile(sourcePath, ios::out | ios::trunc);
    CodeWriter sourceFileWriter(sourceFile);

    headerFileWriter.writeLine(mHeaderFileProlog);
    headerFileWriter.writeHeaderIncludeBlockOpening(headerIncludeBlockText(model, projectName));

    writeStandardIncludFileNamesToHeader(headerFileWriter);
    writeIncludedProtoFileNamesToHeader(headerFileWriter, model);

    writeProtoEnumsToHeader(headerFileWriter, model);

    writeProtoMessagesToHeader(headerFileWriter, model);

    headerFileWriter.writeHeaderIncludeBlockClosing();

    sourceFileWriter.writeLine(mSourceFileProlog);
}

string Protocol::CodeGeneratorCPP::headerIncludeBlockText (const ProtoModel & model, const std::string & projectName) const
{
    string text = projectName;
    if (!text.empty())
    {
        text += "_";
    }

    filesystem::path modelPath(model.fileName());
    text += filesystem::basename(modelPath.filename());
    text += "_h";

    return text;
}

void Protocol::CodeGeneratorCPP::writeStandardIncludFileNamesToHeader (CodeWriter & headerFileWriter) const
{
    headerFileWriter.writeIncludeLibrary("cstdint");
    headerFileWriter.writeIncludeLibrary("string");
    headerFileWriter.writeBlankLine();
}

void Protocol::CodeGeneratorCPP::writeIncludedProtoFileNamesToHeader (CodeWriter & headerFileWriter, const ProtoModel & model) const
{
    auto importedProtoBegin = model.importedProtoNames()->cbegin();
    auto importedProtoEnd = model.importedProtoNames()->cend();
    bool importsFound = false;
    while (importedProtoBegin != importedProtoEnd)
    {
        importsFound = true;
        filesystem::path protoPath(*importedProtoBegin);
        filesystem::path headerPath(filesystem::change_extension(protoPath, mHeaderFileExtension));
        headerFileWriter.writeIncludeProject(headerPath.string());
        ++importedProtoBegin;
    }
    if (importsFound)
    {
        headerFileWriter.writeBlankLine();
    }
}

void Protocol::CodeGeneratorCPP::writeProtoEnumsToHeader (CodeWriter & headerFileWriter, const ProtoModel & model) const
{
    auto protoEnumBegin = model.enums()->cbegin();
    auto protoEnumEnd = model.enums()->cend();
    while (protoEnumBegin != protoEnumEnd)
    {
        auto enumModel = *protoEnumBegin;
        headerFileWriter.writeEnumOpening(enumModel->name());

        auto enumValueBegin = enumModel->enumValues()->cbegin();
        auto enumValueEnd = enumModel->enumValues()->cend();
        bool firstEnumValue = true;
        while (enumValueBegin != enumValueEnd)
        {
            auto enumValueModel = *enumValueBegin;
            if (firstEnumValue)
            {
                headerFileWriter.writeEnumValueFirst(enumValueModel->name(), enumValueModel->value());
                firstEnumValue = false;
            }
            else
            {
                headerFileWriter.writeEnumValueSubsequent(enumValueModel->name(), enumValueModel->value());
            }
            ++enumValueBegin;
        }

        headerFileWriter.writeEnumClosing();
        ++protoEnumBegin;
    }
}

void Protocol::CodeGeneratorCPP::writeProtoMessagesToHeader (CodeWriter & headerFileWriter, const ProtoModel & model) const
{
    auto protoMessageBegin = model.messages()->cbegin();
    auto protoMessageEnd = model.messages()->cend();
    while (protoMessageBegin != protoMessageEnd)
    {
        auto messageModel = *protoMessageBegin;
        headerFileWriter.writeClassOpening(messageModel->name());

        headerFileWriter.writeClassPublic();
        headerFileWriter.writeClassMethodDeclaration(messageModel->name());

        auto messageFieldBegin = messageModel->fields()->cbegin();
        auto messageFieldEnd = messageModel->fields()->cend();
        while (messageFieldBegin != messageFieldEnd)
        {
            auto messageFieldModel = *messageFieldBegin;
            string fieldType = fullTypeName(model, messageFieldModel->fieldType());
            headerFileWriter.writeClassMethodDeclaration(messageFieldModel->name(), fieldType);
            ++messageFieldBegin;
        }

        headerFileWriter.writeClassClosing();
        ++protoMessageBegin;
    }
}

string Protocol::CodeGeneratorCPP::fullTypeName (const ProtoModel & model, const std::string & protoTypeName) const
{
    if (protoTypeName == "bool")
    {
        return "bool";
    }
    if (protoTypeName == "string")
    {
        return "std::string";
    }
    if (protoTypeName == "double")
    {
        return "double";
    }
    if (protoTypeName == "float")
    {
        return "float";
    }
    if (protoTypeName == "int32")
    {
        return "int32_t";
    }
    if (protoTypeName == "int64")
    {
        return "int64_t";
    }
    if (protoTypeName == "uint32")
    {
        return "uint32_t";
    }
    if (protoTypeName == "uint64")
    {
        return "uint64_t";
    }
    if (protoTypeName == "sint32")
    {
        return "int32_t";
    }
    if (protoTypeName == "sint64")
    {
        return "int64_t";
    }
    if (protoTypeName == "fixed32")
    {
        return "int32_t";
    }
    if (protoTypeName == "fixed64")
    {
        return "int64_t";
    }
    if (protoTypeName == "sfixed32")
    {
        return "int32_t";
    }
    if (protoTypeName == "sfixed64")
    {
        return "int64_t";
    }
    if (protoTypeName == "bytes")
    {
        return "bytes";
    }
    return "";
}
