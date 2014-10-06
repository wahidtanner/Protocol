//
//  OneofParserScenarios.cpp
//  Protocol
//
//  Created by Wahid Tanner on 10/5/14.
//

#include <string>

#include "../Submodules/Designer/Designer/Designer.h"

#include "../Protocol/ProtoParser.h"
#include "../Protocol/OneofParser.h"

using namespace std;
using namespace MuddledManaged;

DESIGNER_SCENARIO( OneofParser, "Construction/Normal", "OneofParser can be constructed." )
{
    Protocol::OneofParser parser;
}

DESIGNER_SCENARIO( OneofParser, "Parsing/Normal", "OneofParser can parse simple message." )
{
    shared_ptr<Protocol::ProtoModel> model;

    Protocol::ProtoParser parser("MessageOneof.proto");
    model = parser.parse();

    int messageCount = 0;
    auto begin1 = model->messages()->cbegin();
    auto end1 = model->messages()->cend();
    while (begin1 != end1)
    {
        messageCount++;
        auto message = *begin1;
        verifyEqual("messageOne", message->name());
        verifyTrue(message->fields()->empty());

        int oneofCount = 0;
        auto begin2 = message->oneofs()->cbegin();
        auto end2 = message->oneofs()->cend();
        while (begin2 != end2)
        {
            oneofCount++;
            auto oneof = *begin2;

            int fieldCount = 0;
            auto begin3 = oneof->fields()->cbegin();
            auto end3 = oneof->fields()->cend();
            while (begin3 != end3)
            {
                fieldCount++;
                auto field = *begin3;
                if (fieldCount == 1)
                {
                    verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                    verifyEqual("string", field->fieldType());
                    verifyEqual("sOne", field->name());
                    verifyEqual(1, field->index());
                }
                else if (fieldCount == 2)
                {
                    verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                    verifyEqual("bool", field->fieldType());
                    verifyEqual("bOne", field->name());
                    verifyEqual(2, field->index());
                }
                else if (fieldCount == 3)
                {
                    verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                    verifyEqual("int32", field->fieldType());
                    verifyEqual("iOne", field->name());
                    verifyEqual(3, field->index());
                }
                begin3++;
            }
            verifyEqual(3, fieldCount);
            begin2++;
        }
        verifyEqual(1, oneofCount);
        begin1++;
    }
    verifyEqual(1, messageCount);
}

DESIGNER_SCENARIO( OneofParser, "Parsing/Normal", "OneofParser can parse multiple nested messages with oneofs." )
{
    shared_ptr<Protocol::ProtoModel> model;

    Protocol::ProtoParser parser("MessageOneofMultiple.proto");
    model = parser.parse();

    int message1Count = 0;
    auto begin1 = model->messages()->cbegin();
    auto end1 = model->messages()->cend();
    while (begin1 != end1)
    {
        message1Count++;
        auto message1 = *begin1;
        verifyEqual("messageOne", message1->name());
        verifyEqual(1, message1->fields()->size());

        int oneof1Count = 0;
        auto begin2 = message1->oneofs()->cbegin();
        auto end2 = message1->oneofs()->cend();
        while (begin2 != end2)
        {
            oneof1Count++;
            auto oneof = *begin2;

            int fieldCount = 0;
            auto begin3 = oneof->fields()->cbegin();
            auto end3 = oneof->fields()->cend();
            while (begin3 != end3)
            {
                fieldCount++;
                auto field = *begin3;
                if (oneof1Count == 1)
                {
                    if (fieldCount == 1)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("string", field->fieldType());
                        verifyEqual("sOne", field->name());
                        verifyEqual(1, field->index());
                    }
                    else if (fieldCount == 2)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("bool", field->fieldType());
                        verifyEqual("bOne", field->name());
                        verifyEqual(2, field->index());
                    }
                }
                else if (oneof1Count == 2)
                {
                    if (fieldCount == 1)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("bool", field->fieldType());
                        verifyEqual("bThree", field->name());
                        verifyEqual(3, field->index());
                    }
                    else if (fieldCount == 2)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("int32", field->fieldType());
                        verifyEqual("iThree", field->name());
                        verifyEqual(4, field->index());
                    }
                }
                begin3++;
            }
            verifyEqual(2, fieldCount);
            begin2++;
        }

        int message2Count = 0;
        auto begin4 = message1->messages()->cbegin();
        auto end4 = message1->messages()->cend();
        while (begin4 != end4)
        {
            message2Count++;
            auto message2 = *begin4;
            verifyEqual("messageTwo", message2->name());
            verifyEqual(1, message2->fields()->size());

            int oneof2Count = 0;
            auto begin5 = message2->oneofs()->cbegin();
            auto end5 = message2->oneofs()->cend();
            while (begin5 != end5)
            {
                oneof2Count++;
                auto oneof = *begin5;

                int fieldCount = 0;
                auto begin6 = oneof->fields()->cbegin();
                auto end6 = oneof->fields()->cend();
                while (begin6 != end6)
                {
                    fieldCount++;
                    auto field = *begin6;
                    if (fieldCount == 1)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("string", field->fieldType());
                        verifyEqual("sTwo", field->name());
                        verifyEqual(1, field->index());
                    }
                    else if (fieldCount == 2)
                    {
                        verifyTrue(Protocol::MessageFieldModel::Requiredness::optional == field->requiredness());
                        verifyEqual("int32", field->fieldType());
                        verifyEqual("iTwo", field->name());
                        verifyEqual(2, field->index());
                    }
                    begin6++;
                }
                verifyEqual(2, fieldCount);
                begin5++;
            }
            verifyEqual(1, oneof2Count);
            begin4++;
        }
        verifyEqual(1, message2Count);
        verifyEqual(2, oneof1Count);
        begin1++;
    }
    verifyEqual(1, message1Count);
}
