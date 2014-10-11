//
//  OneofFieldParser.cpp
//  Protocol
//
//  Created by Wahid Tanner on 10/5/14.
//

#include "ParserManager.h"
#include "MessageFieldModel.h"
#include "OneofFieldParser.h"
#include "InvalidProtoException.h"

using namespace std;
using namespace MuddledManaged;

Protocol::OneofFieldParser::OneofFieldParser ()
{
}

bool Protocol::OneofFieldParser::parse (TokenReader::iterator current, TokenReader::iterator end, shared_ptr<ProtoModel> model)
{
    if (current != end)
    {
        // Get the field type.
        if (current->empty())
        {
            throw InvalidProtoException(current.line(), current.column(), "Expected field type.");
        }
        string fieldType = *current;

        // Move to the field name.
        ++current;
        if (current == end || current->empty())
        {
            throw InvalidProtoException(current.line(), current.column(), "Expected field name.");
        }
        string name = *current;

        // Move to the equal.
        ++current;
        if (current == end || *current != "=")
        {
            throw InvalidProtoException(current.line(), current.column(), "Expected = character.");
        }

        // Move to the field index.
        ++current;
        if (current == end || current->empty())
        {
            throw InvalidProtoException(current.line(), current.column(), "Expected field index.");
        }
        unsigned int index = stoi(*current);

        shared_ptr<MessageFieldModel> field(new MessageFieldModel(MessageFieldModel::Requiredness::optional, fieldType, name, index));
        model->addField(current, field);

        // Move to the semicolon or inline options.
        ParserManager * parserMgr = ParserManager::instance();
        ++current;
        if (current == end)
        {
            throw InvalidProtoException(current.line(), current.column(), "Expected ; or [ character.");
        }
        if (*current != ";")
        {
            bool parserFound = false;
            for (auto & parser: *parserMgr->parsers("oneofField"))
            {
                if (parser->parse(current, end, model))
                {
                    parserFound = true;
                    break;
                }
            }
            if (!parserFound)
            {
                throw InvalidProtoException(current.line(), current.column(), "Unexpected option content found.");
            }

            // Move to the semicolon.
            ++current;
            if (current == end || *current != ";")
            {
                throw InvalidProtoException(current.line(), current.column(), "Expected ; character.");
            }
        }
        model->completeField();

        return true;
    }
    return false;
}
