#include "Event.h"
#include "../Text/StringOp.h"

namespace Util {
    namespace Event {
        MessageHandler::MessageHandler()
        : isValid_(false)
        , owner_(0)
        , handler_(0)
        , nexts_(0) {
        }

        MessageHandler::MessageHandler(Component const* const owner, Handler const handler)
        : isValid_(true)
        , owner_(const_cast<Component* const>(owner))
        , handler_(handler)
        , nexts_(0) {
        }

        MessageHandler::~MessageHandler() {
        }

        MessageHandler const MessageHandler::operator=(MessageHandler const& rhs) {
            isValid_ = true;
            owner_ = rhs.owner_;
            handler_ = rhs.handler_;
            nexts_ = 0;
            return *this;
        }

        void MessageHandler::Clear() {
            MessageHandler* handler = nexts_;
            MessageHandler* tempHandler = 0;
            while (handler != 0) {
                tempHandler = handler;
                handler = handler->nexts_;
                delete tempHandler;
            }
        }

        void MessageHandler::operator()(Component const* const sender, MessageArgs const* const args) {
            if (isValid_) {
                (owner_->*handler_)(sender, args);
                MessageHandler* handler = nexts_;
                while (handler != 0) {
                    if (handler->IsValid()) {
                        (*handler)(sender, args);
                    }
                    handler = handler->nexts_;
                }
            }
        }

        void MessageHandler::operator+=(MessageHandler const* const next) {
            if (isValid_) {
                MessageHandler* handler = nexts_;
                while (handler != 0) {
                    if (handler->nexts_ == 0) {
                        handler->nexts_ = const_cast<MessageHandler* const>(next);
                        break;
                    } else {
                        handler = handler->nexts_;
                    }
                }
            } else {
                owner_ = next->owner_;
                handler_ = next->handler_;
                isValid_ = true;
            }
        }

        void MessageHandler::operator-=(MessageHandler const* const next) {
            MessageHandler* handler = nexts_;
            while (handler != 0) {
                if (*(handler->nexts_) == *next) {
                    handler->nexts_ = handler->nexts_->nexts_;
                    break;
                } else {
                    handler = handler->nexts_;
                }
            }
        }

        bool const MessageHandler::operator==(MessageHandler const& rhs) {
            bool result = false;
            if ((owner_ == rhs.owner_) && (handler_ == rhs.handler_)) {
                result = true;
            }
            return result;
        }

        bool MessageHandler::IsValid() {
            return isValid_;
        }
    }
}
