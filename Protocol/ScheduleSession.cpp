#include "../Data/SchedulerData.h"
#include "ScheduleSession.h"

ScheduleSession::ScheduleSession(void)
{
}

ScheduleSession::~ScheduleSession(void)
{
}

void ScheduleSession::SetParameter(std::string const& beginDate, std::string const& endDate) {
	beginDate_ = beginDate;
	endDate_ = endDate;
}

void ScheduleSession::selfProcess_(ResponseMessage const& response) const {
	/*
<?xml version="1.0" encoding="UTF-8"?>
<schedullistresponse>
	<count>2</count>
	<pack>
		<date>20090909</date>
		<time>121212</time>
		<creator>张三</creator>
		<content>生日</content>
	</pack>
	<pack>
		<date>20090909</date>
		<time>121201</time>
		<creator>张四</creator>
		<content>生日</content>
	</pack>
</shcedullistresponse>
	*/

    size_t o = 0;
	std::string pack = findContent_(response.Body, "pack", o);
    while (pack != "") {
        size_t offset = 0;
		//@@
        //std::string date = findContent_(pack, "date", offset);
        //std::string time = findContent_(pack, "time", offset);
        //std::string creator = findContent_(pack, "creator", offset);
        std::string date = findContent_(pack, "Date", offset);
        std::string time = findContent_(pack, "Time", offset);
        std::string creator = findContent_(pack, "Creator", offset);
		//@@
        std::string content = findContent_(pack, "content", offset);
		Data::Scheduler s;
		s.startTime(Util::StringOp::ToTimestamp(date + time));
		s.tipsType(Data::ttNet);
		s.what(content);
		s.Insert();
		pack = findContent_(response.Body, "pack", o);
    }
}

std::string const ScheduleSession::resourceName_() const {
    return "ZF3G/tic/schedullist";
}

std::string const ScheduleSession::additionParameter_() const {
    return "&begindate=" + beginDate_ + "&enddate=" + endDate_;
}
