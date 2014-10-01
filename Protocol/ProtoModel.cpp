//
//  ProtoModel.cpp
//  Protocol
//
//  Created by Wahid Tanner on 9/16/14.
//

#include "ProtoModel.h"

using namespace std;
using namespace MuddledManaged;

Protocol::ProtoModel::ProtoModel ()
{
}

std::string Protocol::ProtoModel::currentPackage () const
{
    return package();
}

void Protocol::ProtoModel::setCurrentPackage (const string & package)
{
    setPackage(package);
}

void Protocol::ProtoModel::addEnum (EnumModelCollection::value_type enumeration)
{
    if (mMessageQueue.empty())
    {
        mEnums.push_back(enumeration);
    }
    else
    {
        mMessageQueue.back()->addEnum(enumeration);
    }
}

void Protocol::ProtoModel::completeEnum ()
{
}

void Protocol::ProtoModel::addMessage (MessageModelCollection::value_type message)
{
    if (mMessageQueue.empty())
    {
        mMessages.push_back(message);
    }
    else
    {
        mMessageQueue.back()->addMessage(message);
    }
    mMessageQueue.push_back(message);
}

void Protocol::ProtoModel::completeMessage ()
{
    mMessageQueue.pop_back();
}

Protocol::ProtoModel::EnumModelCollection::const_iterator Protocol::ProtoModel::cbeginEnum () const
{
    return mEnums.cbegin();
}

Protocol::ProtoModel::EnumModelCollection::const_iterator Protocol::ProtoModel::cendEnum () const
{
    return mEnums.cend();
}

Protocol::ProtoModel::MessageModelCollection::const_iterator Protocol::ProtoModel::cbeginMessage () const
{
    return mMessages.cbegin();
}

Protocol::ProtoModel::MessageModelCollection::const_iterator Protocol::ProtoModel::cendMessage () const
{
    return mMessages.cend();
}
