#include "../Util/StringOp.h"
//#include "../Util/XMLParser.h"
#include "TaskSession.h"

TaskSession::TaskSession(void)
{
}

TaskSession::~TaskSession(void)
{
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
	size_t offset = 0;
	std::string v = findContent_(response.Body, "taskcount", offset);
    size_t count = Util::StringOp::ToInt(v);
    if (count == 5) {
        //retry
    }
}

std::string const TaskSession::resourceName_() const {
    return "ZF3G/tic/tasks";
}
