//
//  ProtoModel.h
//  Protocol
//
//  Created by Wahid Tanner on 9/16/14.
//

#ifndef Protocol_ProtoModel_h
#define Protocol_ProtoModel_h

#include <memory>
#include <string>
#include <vector>

#include "Packageable.h"
#include "EnumModel.h"
#include "MessageModel.h"

namespace MuddledManaged
{
    namespace Protocol
    {
        class ProtoModel : private Packageable
        {
        public:
            typedef std::vector<std::shared_ptr<EnumModel>> EnumModelCollection;
            typedef std::vector<std::shared_ptr<MessageModel>> MessageModelCollection;

            ProtoModel ();

            std::string currentPackage () const;
            void setCurrentPackage (const std::string & package);

            void addEnum (EnumModelCollection::value_type enumeration);
            void completeEnum ();

            void addMessage (MessageModelCollection::value_type message);
            void completeMessage ();

            EnumModelCollection::const_iterator cbeginEnum () const;
            EnumModelCollection::const_iterator cendEnum () const;

            MessageModelCollection::const_iterator cbeginMessage () const;
            MessageModelCollection::const_iterator cendMessage () const;

        private:
            EnumModelCollection mEnums;
            MessageModelCollection mMessages;
            MessageModelCollection mMessageQueue;
        };

    } // namespace Protocol

} // namespace MuddledManaged

#endif // Protocol_ProtoModel_h
