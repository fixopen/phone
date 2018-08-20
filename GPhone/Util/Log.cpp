#include "Log.h"

namespace Util {
    namespace Log {
        Entry const Processor::Process(Entry const& entry) {
            for (size_t i = 0; i < nexts_.size(); ++i) {
                nexts_[i]->Process(entry);
            }
            return entry;
        }
        
        void Processor::AddNext(Processor* const next) {
            nexts_.push_back(next);
        }

        void Processor::RemoveNext(Processor* const next) {
            //nexts_.erase(next);
            delete next;
        }

        Processor::~Processor() {
        }

        Entry const Source::Process(Entry const& dummyEntry) {
            Entry entry = parser_->Parse(receiver_->Read());
            return Processor::Process(entry);
        }

        void Source::SetParser(Parser* const parser) {
            parser_ = parser;
        }
        
        void Source::SetReceiver(Receiver* const receiver) {
            receiver_ = receiver;
        }
        
        Source::~Source() {
            delete receiver_;
            delete parser_;
        }

        Entry const Sink::Process(Entry const& entry) {
            destination_->Write(formatter_->Format(entry));
            return Processor::Process(entry);
        }

        void Sink::SetFormatter(Formatter* const formatter) {
            formatter_ = formatter;
        }
        
        void Sink::SetDestination(Destination* const destination) {
            destination_ = destination;
        }

        Sink::~Sink() {
            delete formatter_;
            delete destination_;
        }

        bool const Filter::IsAccept(Entry const& entry) {
            bool result = false;
            if (entry.level < level_) {
                result = true;
            }
            return result;
        }

        void Filter::SetLevel(Entry::Level const level) {
            level_ = level;
        }

        Filter::~Filter() {
        }

        //class Forker : public Processor { //same as processor
        //public:
        //    virtual ~Forker();
        //};

        std::string const Receiver::Read() {
            std::string result;
            //read from FILE*, fd, socket, Handle, and so on...
            return result;
        }

        NetworkReceiver::NetworkReceiver(std::string const& ip, unsigned short const port) {
            socket_ = 0;
        }

        std::string const NetworkReceiver::Read() {
            std::string result;
            //read from socket_
            return result;
        }

        std::string const Formatter::Format(Entry const& entry) {
            std::string result = entry.what;
            //convert to string
            return result;
        }

        std::string const SQLFormatter::Format(Entry const& entry) {
            std::string result;
            //convert to sql insert command
            return result;
        }

        Entry const Parser::Parse(std::string const& value) {
            Entry result(value);
            //parse from string
            return result;
        }

        Entry const SQLParser::Parse(std::string const& sql) {
            Entry result(sql);
            //parse from sql insert command
            return result;
        }

        void Destination::Write(std::string const& value) {
            //write string to destination, include FILE*, fd, socket, Handle, and so on...
        }

        FileDestination::FileDestination(std::string const& filename) : isConsole_(false) {
            if (filename != "") {
                file_ = fopen(filename.c_str(), "w+");
            } else {
                isConsole_ = true;
                file_ = stdout;
            }
        }

        FileDestination::~FileDestination() {
            if (!isConsole_) {
                fclose(file_);
            }
        }

        void FileDestination::Write(std::string const& value) {
            fprintf(file_, "%s\n", value.c_str());
        }

        MemoryDestination::MemoryDestination(size_t count) {
            //limit
        }

        void MemoryDestination::Write(std::string const& value) {
            contents_.push_back(value);
        }

        NetworkDestination::NetworkDestination(std::string const& ip, unsigned short const port) {
            socket_ = 0; //socket();
            //...
        }

        void NetworkDestination::Write(std::string const& value) {
            //send(socket_, value.c_str(), value.length(), 0, 0);
        }

        DatabaseDestination::DatabaseDestination(std::string const& databaseName, std::string const& tableName) {
            database_ = 0;
        }

        void DatabaseDestination::Write(std::string const& value) {
            //writeToDb(database_, ...);
        }

        namespace {
            std::vector<Source*> sources;
            Processor* firstNode = 0;
            int currentId = 0;
        }

        Processor* const FindNodeById(Processor* const node, int const id) {
            Processor* result = 0;
            if (node->id_ == id) {
                result = node;
            } else {
                for (size_t i = 0; i < node->nexts_.size(); ++i) {
                    result = FindNodeById(node->nexts_[i], id);
                }
            }
            return result;
        }

        int const AddSource(Source* const processor) {
            ++currentId;
            processor->id_ = currentId;
            sources.push_back(processor);
            return currentId;
        }

        int const AddProcess(Processor* const processor, int const previousNode) {
            ++currentId;
            processor->id_ = currentId;
            if (firstNode) {
                Processor* node = FindNodeById(firstNode, previousNode);
                node->AddNext(processor);
            } else {
                firstNode = processor;
                for (size_t i = 0; i < sources.size(); ++i) {
                    sources[i]->AddNext(firstNode);
                }
            }
            return currentId;
        }

        //int const AddProcess(Processor* const processor, int const previousNode) {
        //    ++currentId;
        //    processor->id_ = currentId;
        //    Source* source = dynamic_cast<Source*>(processor);
        //    if (source) {
        //        sources.push_back(source);
        //    } else {
        //        if (firstNode) {
        //            Processor* node = FindNodeById(firstNode, previousNode);
        //            node->AddNext(processor);
        //        } else {
        //            firstNode = processor;
        //            for (size_t i = 0; i < sources.size(); ++i) {
        //                sources[i]->AddNext(firstNode);
        //            }
        //        }
        //    }
        //    return currentId;
        //}

        Processor* const RemoveProcess(int const id) {
            Processor* node = FindNodeById(firstNode, id);
            Processor* previousNode = 0;
            previousNode->RemoveNext(node);
            for (size_t i = 0; i < node->nexts_.size(); ++i) {
                previousNode->AddNext(node->nexts_[i]);
            }
            return node;
        }

        /*int const AddSource(Source* const source, Parser* const parser, Receiver* const receiver) {
            source->SetParser(parser);
            source->SetReceiver(receiver);
            ++currentId;
            source->id_ = currentId;
            if (firstNode) {
                source->AndNext(firstNode);
            }
            return currentId;
        }

        int const AddSink(Sink* const sink, Formatter* const formatter, Destination* const destination, int const previousNode) {
            sink->SetFormatter(formatter);
            sink->SetDestination(destination);
            ++currentId;
            sink->id_ = currentId;
            Processor* node = FindNodeById(firstNode, previousNode);
            node->AppendNext(sink);
            return currentId;
        }*/

        void Log(Entry const& entry) {
            firstNode->Process(entry);
        }

        void StartLog() {
            Entry dummyEntry("dummy");
            for (size_t i = 0; i < sources.size(); ++i) {
                sources[i]->Process(dummyEntry);
            }
        }
    }
#if 0
	std::string DataAccess<Log>::tableName_ = "log";
	sqlite3* DataAccess<Log>::db_ = GetDB();
    int DataAccess<Log>::count_ = 0;
	int DataAccess<Log>::offset_ = 0;
	Indication DataAccess<Log>::indication_ = 0;

	Log::Log()
		: level_(llError)
		, who_()
		, when_() //Util::Timestamp::GetCurrentTime()
		//, what_("")
		//, code_(ecSystemError)
		, value_(0.0)
	{
	}

    std::vector<Util::shared_ptr<Log> > Log::GetFromDatabase(std::string const& filter, std::string const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
		return DataAccess<Log>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
	}

	void Log::Update() const {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [level] = ";
        cmd += Util::StringOp::FromInt(level_);
        cmd += ", [who] = '";
        cmd += who_;
        cmd += ", [when] = '";
        cmd += Util::StringOp::FromTimestamp(when_);
        cmd += "', [code] = ";
        cmd += Util::StringOp::FromInt(code_);
        cmd += ", [value] = '";
        cmd += Util::StringOp::FromDouble(value_);
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void Log::Insert() {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( level, who, when, what, code, value ) VALUES ( ";
        cmd += Util::StringOp::FromInt(level_);
        cmd += ", '";
        cmd += who_;
        cmd += "', '";
        cmd += Util::StringOp::FromTimestamp(when_);
        cmd += "', '";
        cmd += what_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(code_);
        cmd += ", ";
        cmd += Util::StringOp::FromDouble(value_);
        cmd += " )";
        ExecCommand(cmd);
		id(GetCurrentId());
	}
	
	void Log::Remove(std::string filter) {
		DataAccess<Log>::RemoveDatasByFilter(filter);
	}

	std::string Log::ToString(void) {
		std::string result = "<ITEM>\n\t<LEVEL>";
		/*
		result += Util::StringOp::FromEnumLogLevel(level);
		result += "</LEVEL>\n\t<WHO>";
		result += who;
		result += "</WHO>\n\t<TIME>";
		result += Util::StringOp::FromTimestamp(when); //yyyy:mm:dd:hh:mm:ss;
		result += "</TIME>\n\t<CODE>";
		result += Util::StringOp::FromEnumErrorCode(code);
		result += "</CODE>\n\t<DESCRIPTION>";
		result += what;
		result += "</DESCRIPTION>\n\t<VALUE>";
		result += Util::StringOp::FromDouble(value);
		result += "</VALUE>\n</ITEM>";
		*/
		return result;
	}

	void Log::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Log> item) {
        item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
        item->level_ = static_cast<LogLevel>(atoi(argv[getIndexByName(argc, columnName, "level")]));
        item->who_ = argv[getIndexByName(argc, columnName, "who")];
        item->when_ = Util::StringOp::ToTimestamp(std::string((char*)argv[getIndexByName(argc, columnName, "when")]));
        item->what_ = argv[getIndexByName(argc, columnName, "what")];
        item->code_ = static_cast<LogCode>(atoi(argv[getIndexByName(argc, columnName, "code")]));
        item->value_ = atof(argv[getIndexByName(argc, columnName, "value")]);
	}
#endif
}
