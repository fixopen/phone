#include "../Util/StringOp.h"
//#include "../Util/XMLParser.h"
#include "TaskSession.h"
#include "MediaDownloadSession.h"
#include "MemberManagerSession.h"
#include "ScheduleSession.h"
#include "WeatherSession.h"

TaskSession::TaskSession(void)
{
}

TaskSession::~TaskSession(void)
{
}

namespace {
	std::string const getNameByLink(std::string const& link) {
		std::string result = link;
		size_t namePos = link.find_last_of("/");
		if (namePos != std::string::npos) {
			result = link.substr(namePos);
		}
		return result;
	}
}

void TaskSession::selfProcess_(ResponseMessage const& response) const {
    //Util::DocSource* source = new Util::MemDocSource(Util::StringOp::FromUTF8(response.Body));
    //Util::XMLParser parser;
    //Util::shared_ptr<Xml::Document> doc = parser.Parse(source);
    //delete source;
    /*
<?xml version="1.0" encoding="UTF-8"?>
<tasklist>
	<taskcount>2</taskcount>
	<taskitem>
		<tasktype>01</tasktype>
		<taskid>1350123456700001</taskid>
		<currentversion>1.0</currentversion>
		<updateversion>2.0</updateversion>
		<updateinfo>有最新版本V2.0了</updateinfo>
		<neededsize>100100</neededsize>
		<packetsize>60200</packetsize>
		<link>http://[server-url]/install/12382/kj.jad</link>
	</taskitem>
	<taskitem>
		<tasktype>02</tasktype>
		<taskid>1391234567800290</taskid>
		<servicecode>001</servicecode>
		<msgid>1234567890</msgid>
		<sendername>unknow user</sendername>
		<msgtype>picture</msgtype>
		<subject>unknown subject</subject>
		<contents>
			<content>
				<cid>5</cid>
				<ctype>jpg</ctype>
				<csize>126573</csize>
				<link>http://10.1.6.240/pic/moon.jpg</link>
			</content>
			<content>
				<cid>6</cid>
				<ctype>jpg</ctype>
				<csize>196600</csize>
				<link>http://10.1.6.240/pic/tree.jpg</link>
			</content>
		</contents>
	</taskitem>
    <taskitem>
		<tasktype>03</tasktype>
		<taskid>1350123456700003</taskid>
		<servicecode>002</servicecode>
		<link>http://[server-url]/weatherreport</link>
	</taskitem>
</tasklist>
    */
    //std::string tagName = "taskcount";
    //size_t startPos = response.Body.find("<" + tagName + ">") + tagName.length() + 2;
    //size_t stopPos = response.Body.find("</" + tagName + ">", startPos);
    //std::string v = response.Body.substr(startPos, stopPos - startPos);
    //std::string tagName = "taskcount";
    //size_t startPos = response.Body.find("<" + tagName + ">") + tagName.length() + 2;
    //size_t stopPos = response.Body.find("</" + tagName + ">", startPos);
    //std::string v = response.Body.substr(startPos, stopPos - startPos);
	size_t offset = 0;
	std::string v = findContent_(response.Body, "taskcount", offset);
    size_t count = Util::StringOp::ToInt(v);
    if (count == 5) {
        //retry
    }
    std::vector<std::string> tasks;
    offset = 0;
    std::string task = findContent_(response.Body, "taskitem", offset);
    while (task != "") {
        tasks.push_back(task);
        task = findContent_(response.Body, "taskitem", offset);
    }
	//@@
    /*
	01：软件升级
	02：获取多媒体内容（图片、音频、视频等）
	03：获取图文咨询
	04：菜单更新（暂不使用）
	05：应用更新（暂不使用）
	06：业务更新（暂不使用）
	07：内容删除
	08：家庭成员信息更新
	09：家庭好友信息更新
	10：家庭日程信息更新
	11：设备参数更新
	*/
	/*
	01：软件升级
	02：获取多媒体内容
	03：日程下载
	04：家庭列表

	06：天气预报
	07：信息机内容删除
	08：图文资讯
	*/
	//@@
    for (size_t i = 0; i < tasks.size(); ++i) {
        size_t offset = 0;
        std::string type = findContent_(tasks[i], "tasktype", offset);
		if (type == "01") { //software
			int i = 0;
		} else if (type == "02") { //multimedia download
            /*
            1：图片
            2：
            */
            offset = 0;
            std::string msgtype = findContent_(tasks[i], "msgtype", offset);
            //if (msgtype == "1" || msgtype == "picture") { //picture type
            offset = 0;
            std::string taskid = findContent_(tasks[i], "taskid", offset);
            offset = 0;
            std::string msgid = findContent_(tasks[i], "msgid", offset);
            offset = 0;
            std::string contents = findContent_(tasks[i], "contents", offset);
            std::vector<std::string> contentArray;
            size_t offset = 0;
            std::string content = findContent_(contents, "content", offset);
            while (content != "") {
                contentArray.push_back(content);
                content = findContent_(contents, "content", offset);
            }
            for (size_t i = 0; i < contentArray.size(); ++i) {
                size_t offset = 0;
                std::string cid = findContent_(contentArray[i], "cid", offset);
                offset = 0;
                std::string ctype = findContent_(contentArray[i], "ctype", offset);
				if ((ctype == "jpg") || (ctype == "gif") || (ctype == "bmp") || (ctype == "png")) {
					msgtype = "1";
				} else if ((ctype == "mp4") || (ctype == "avi") || (ctype == "asf") || (ctype == "dat")) {
					msgtype = "3";
				}
                offset = 0;
                std::string link = findContent_(contentArray[i], "link", offset);
                MediaDownloadSession d;
                d.SetParameters(msgtype, taskid, msgid, cid, getNameByLink(link));
                d.Process();
            }
            //}
        } else if (type == "03") { //mminfo\schedule
			int i = 0;
			ScheduleSession* schedule = new ScheduleSession();
			schedule->SetParameter("20091001", "20091101");
			schedule->Process();
			delete schedule;
		} else if (type == "04") { //menu\member info
			int i = 0;
		} else if (type == "05") { //application
			int i = 0;
		} else if (type == "06") { //biz\weather
			int i = 0;
			std::string taskid = findContent_(tasks[i], "taskid", offset);
			WeatherSession* weather = new WeatherSession();
			weather->SetParameter(taskid);
			weather->Process();
			delete weather;
		} else if (type == "07") { //delete
			int i = 0;
		} else if (type == "08") { //member info\mminfo
			//MemberManagerSession m;
			//m.SetParameter(familyId);
			//m.Process();
			int i = 0;
		} else if (type == "09") { //friend info
			int i = 0;
		} else if (type == "10") { //schedule
			int i = 0;
            ScheduleSession s;
            //s.SetParameter(beginDate, endDate);
            s.Process();
		} else if (type == "11") { //device parameters
			int i = 0;
		}
    }
}

std::string const TaskSession::resourceName_() const {
    return "ZF3G/tic/tasks";
}
