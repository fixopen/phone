#include "TaskReportSession.h"

namespace Protocol {
    TaskReportSession::TaskReportSession(void) {
    }

    TaskReportSession::~TaskReportSession(void) {
    }

    void TaskReportSession::selfProcess_(Util::ResponseMessage const & response) const {
    }

    std::string const TaskReportSession::resourceName_() const {
        return "ZF3G/tic/taskstatus";
    }

    std::string const TaskReportSession::additionParameter_() const {
        return "&taskid=12345&status=11&cid=123";
    }
}
