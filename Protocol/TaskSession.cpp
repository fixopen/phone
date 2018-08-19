#include "TaskSession.h"
#include "../Util/StringOp.h"

TaskSession::TaskSession(void)
{
}

TaskSession::~TaskSession(void)
{
}

void TaskSession::selfProcess_(ResponseMessage const& response) const {
    std::string tagName = "taskcount";
    size_t startPos = response.Body.find("<" + tagName + ">") + tagName.length() + 2;
    size_t stopPos = response.Body.find("</" + tagName + ">", startPos);
    std::string v = response.Body.substr(startPos, stopPos - startPos);
    size_t count = Util::StringOp::ToInt(v);
    if (count == 5) {
        //retry
    }
}

std::string const TaskSession::resourceName_() const {
    return "ZF3G/tic/tasks";
}
