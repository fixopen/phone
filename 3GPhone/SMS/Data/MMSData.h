#include <string>
using std::string;

namespace MMS
{
    class MMSData
    {
    public:
        MMSData();
        ~MMSData();
        MMSData(const MMSData& mms);

        struct MMS_SIZE
        {
            unsigned int width;
            unsigned int height;
        };

        enum MMS_REGION_TYPE
        {
            rtImage,
            rtText,
            rtVideo,
        };

        enum MMS_REGION_FIT
        {
            rfHidden,
            rfFill,
            rfMeet,
            rfScroll,
            rfSlice,
        };

        struct MMS_REGION
        {
            MMS_REGION_TYPE id;
            unsigned int top;
            unsigned int left;
            unsigned int width;
            unsigned int height;
            string fit;
        };

        struct MMS_LAYOUT
        {
            MMS_SIZE root_layout;
            map<string, MMS_REGION> regions;
        };

        enum MMS_SRCTYPE
        {
            stImge,
            stText,
            stAudio,
            stVideo
        };

        struct MMS_SRC
        {
            MMS_SRCTYPE type;
            string name;
            string region;
            unsigned int begin;
            unsigned int end;
            unsigned int dur;
            string alt;
        };

        struct MMS_PAR
        {
            unsigned int duration;
            vector<MMS_SRC> srcs;
        };

        struct MMS_CONTENT
        {
            //head
            char MessageType;//m-send-req = <Octet 128>
            string TransactionID;
            char MMSVersion;//MMS-version-value = Short-integer
            string RecipientAddress;//To-value = Encoded-string-value
            string ContentType;
            string SenderAddress;//Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
            unsigned int TimeOfExpiry;//Expiry-value = Value-length (Absolute-token Date-value | Relative-token Delta-seconds-value)Absolute-token = <Octet 128>	Relative-token = <Octet 129>
            //	char DeliveryReport;//Delivery-report-value = Yes | No	Yes = <Octet 128>		No = <Octet 129>
            //	char MMState;//Status-value = Expired | Retrieved | Rejected | Deferred | Unrecognised	Expired = <Octet 128>		Retrieved = <Octet 129>		Rejected = <Octet 130>		Deferred = <Octet 131>		Unrecognised = <Octet 132>		The value Unrecognized is reserved for version management purpose only.
            string Subject;//Encoded-string-value


            string MessageID;//Message-ID-value = Text -string
            char MessageClass;//Message-class-value = Class-identifier | Token-text	Class-identifier = Personal | Advertisement | Informational | Auto		Personal = <Octet 128>		Advertisement = <Octet 129>		Informational = <Octet 130>		Auto = <Octet 131>
            unsigned int DateAndTime;//Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
            char RequestStatus;

            //body
            MMS_LAYOUT layout;
            vector<MMS_PAR> pars;
            string smil;//smil file
            vector<string> content;//media file
        };
        MMS_CONTENT content;

        bool isRead;

        enum Type
        {
            tpReceive,
            tpSend,
            tpUnSend
        };
        Type type;		
        ///////////////////////////////////////////////////////////////////////////////////
        void AddRootLayout(unsigned int width, unsigned int height)
        {
            content.layout.root_layout.width = width;
            content.layout.root_layout.height = height;
        }

        void AddRegion(MMS_REGION_TYPE id, unsigned int top, unsigned int left, unsigned int width, unsigned int height, MMS_REGION_FIT fit)
        {
            char* types[] = {"Image", "Text", "Video"};
            char* fits[] = {"hidden", "fill", "meet", "scroll", "slice"};

            MMS_REGION region;
            region.top = top;
            region.left = left;
            region.width = width;
            region.height = height;
            region.fit = fits[fit];

            content.layout.regions[types[id]] = region;
        }

        void AddFrame(string imageFile, string textFile, string audioFile, unsigned int dur)
        {
            if (!imageFile.empty())
            {
                content.content.push_back(imageFile);
                if (imageFile.rfind("\\") != string::npos)
                {
                    imageFile = imageFile.substr(imageFile.rfind("\\") + 1);
                }
            }
            if (!textFile.empty())
            {
                content.content.push_back(textFile);
                if (textFile.rfind("\\") != string::npos)
                {
                    textFile = textFile.substr(textFile.rfind("\\") + 1);
                }
            }
            if (!audioFile.empty())
            {
                content.content.push_back(audioFile);
                if (audioFile.rfind("\\") != string::npos)
                {
                    audioFile = audioFile.substr(audioFile.rfind("\\") + 1);
                }
            }

            MMS_PAR par;
            par.duration = dur;

            if (!imageFile.empty())
            {
                MMS_SRC image;
                image.name = imageFile;
                image.region = "Image";
                par.srcs.push_back(image);
            }

            if (!textFile.empty())
            {
                MMS_SRC text;
                text.name = textFile;
                text.region = "Text";
                par.srcs.push_back(text);
            }

            if (!audioFile.empty())
            {
                MMS_SRC audio;
                audio.name = audioFile;
                par.srcs.push_back(audio);
            }
            content.pars.push_back(par);
        }
    };
}