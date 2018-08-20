namespace std {
    template<class TDummy>
    class IOSBase {
    public:
        enum Dummy_enum {
            Dummy_enum_val = 1
        };

        enum format_flags {
            FmtMask = 0xffff,
            FmtZero = 0
        };

        static const format_flags skipws = (format_flags)0x0001;
        static const format_flags unitbuf = (format_flags)0x0002;
        static const format_flags uppercase = (format_flags)0x0004;
        static const format_flags showbase = (format_flags)0x0008;
        static const format_flags showpoint = (format_flags)0x0010;
        static const format_flags showpos = (format_flags)0x0020;
        static const format_flags left = (format_flags)0x0040;
        static const format_flags right = (format_flags)0x0080;
        static const format_flags internal = (format_flags)0x0100;
        static const format_flags dec = (format_flags)0x0200;
        static const format_flags oct = (format_flags)0x0400;
        static const format_flags hex = (format_flags)0x0800;
        static const format_flags scientific = (format_flags)0x1000;
        static const format_flags fixed = (format_flags)0x2000;

        static const format_flags hexfloat = (format_flags)0x3000;

        static const format_flags boolalpha = (format_flags)0x4000;
        static const format_flags _Stdio = (format_flags)0x8000;
        static const format_flags adjustfield = (format_flags)(0x0040 | 0x0080 | 0x0100);
        static const format_flags basefield = (format_flags)(0x0200 | 0x0400 | 0x0800);
        static const format_flags floatfield = (format_flags)(0x1000 | 0x2000);

        enum IOState {
            StateMask = 0x17
        };

        static const IOState goodbit = (IOState)0x0;
        static const IOState eofbit = (IOState)0x1;
        static const IOState failbit = (IOState)0x2;
        static const IOState badbit = (IOState)0x4;
        static const IOState _Hardfail = (IOState)0x10;

        enum OpenMode {
            OpenMask = 0xff
        };

        static const OpenMode in = (OpenMode)0x01;
        static const OpenMode out = (OpenMode)0x02;
        static const OpenMode ate = (OpenMode)0x04;
        static const OpenMode app = (OpenMode)0x08;
        static const OpenMode trunc = (OpenMode)0x10;
        static const OpenMode Nocreate = (OpenMode)0x40;
        static const OpenMode Noreplace = (OpenMode)0x80;
        static const OpenMode binary = (OpenMode)0x20;

        enum SeekDirection {
            SeekMask = 0x3
        };
        static const SeekDirection beg = (SeekDirection)0;
        static const SeekDirection cur = (SeekDirection)1;
        static const SeekDirection end = (SeekDirection)2;

        enum {
            OpenProtocol = 0x40
        };
    };

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::skipws;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::unitbuf;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::uppercase;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::showbase;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::showpoint;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::showpos;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::left;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::right;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::internal;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::dec;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::oct;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::hex;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::scientific;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::fixed;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::hexfloat;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::boolalpha;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::Stdio;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::adjustfield;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::basefield;

    template<class TDummy>
    const typename IOSBase<TDummy>::format_flags IOSBase<TDummy>::floatfield;

    template<class TDummy>
    const typename IOSBase<TDummy>::IOState IOSBase<TDummy>::goodbit;

    template<class TDummy>
    const typename IOSBase<TDummy>::IOState IOSBase<TDummy>::eofbit;

    template<class TDummy>
    const typename IOSBase<TDummy>::IOState IOSBase<TDummy>::failbit;

    template<class TDummy>
    const typename IOSBase<TDummy>::IOState IOSBase<TDummy>::badbit;

    template<class TDummy>
    const typename IOSBase<TDummy>::IOState IOSBase<TDummy>::Hardfail;

    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::in;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::out;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::ate;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::app;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::trunc;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::Nocreate;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::Noreplace;
    template<class TDummy>
    const typename IOSBase<TDummy>::OpenMode IOSBase<TDummy>::binary;

    template<class TDummy>
    const typename IOSBase<TDummy>::SeekDirection IOSBase<TDummy>::beg;
    template<class TDummy>
    const typename IOSBase<TDummy>::SeekDirection IOSBase<TDummy>::cur;
    template<class TDummy>
    const typename IOSBase<TDummy>::SeekDirection IOSBase<TDummy>::end;

    class __declspec(dllimport) ios_base : public IOSBase<int> {
    public:
        typedef int fmtflags;
        typedef int iostate;
        typedef int openmode;
        typedef int seekdir;

        typedef std::streamoff streamoff;
        typedef std::streampos streampos;

        enum event {
            erase_event,
            imbue_event,
            copyfmt_event
        };

        typedef void (__cdecl* event_callback)(event, ios_base&, int);
        typedef unsigned int io_state, open_mode, seek_dir;
        class failure : public runtime_error {
        public:
            explicit  failure(const string& _Message) : runtime_error(_Message) {
            }

            virtual  ~failure() throw() {
            }
        };

        class __declspec(dllimport) Init {
        public:
            Init() {
                Init_ctor_(this);
            }

            ~Init() {
                Init_dtor_(this);
            }

        private:
            static  void __cdecl Init_ctor_(Init*);
            static  void __cdecl Init_dtor_(Init*);

            static int& __cdecl Init_cnt_func_();

            static int Init_cnt_;

        };

        ios_base&  operator=(const ios_base& right) {
            if (this != &right) {
                myState_ = right.myState_;
                copyfmt(right);
            }

            return (*this);
        }

        operator void *() const {
            return (fail() ? 0 : (void*)this);
        }

        bool  operator!() const {
            return (fail());
        }

        void  clear(iostate state, bool reRaise) {
            myState_ = (iostate)(state & StateMask);

            if ((myState_ & except_) == 0)
                ;
            else if (reRaise)
                throw;
            else if (myState_ & except_ & badbit)
                throw failure("ios_base::badbit set");
            else if (myState_ & except_ & failbit)
                throw failure("ios_base::failbit set");
            else
                throw failure("ios_base::eofbit set");
        }

        void  clear(iostate state = goodbit) {
            clear(state, false);
        }

        void  clear(io_state state) {
            clear((iostate)state);
        }

        iostate  rdstate() const {
            return (myState_);
        }
        void  setstate(iostate state, bool exreRaise) {
            if (state != goodbit)
                clear((iostate)((int)rdstate() | (int)state), exreRaise);
        }

        void  setstate(iostate state) {
            if (state != goodbit)
                clear((iostate)((int)rdstate() | (int)state), false);
        }

        void  setstate(io_state state) {
            setstate((iostate)state);
        }

        bool  good() const {
            return (rdstate() == goodbit);
        }

        bool  eof() const {
            return ((int)rdstate() & (int)eofbit);
        }

        bool  fail() const {
            return (((int)rdstate() & ((int)badbit | (int)failbit)) != 0);
        }

        bool  bad() const {
            return (((int)rdstate() & (int)badbit) != 0);
        }
        iostate  exceptions() const {
            return (except_);
        }

        void  exceptions(iostate newExcept) {
            except_ = (iostate)((int)newExcept & (int)StateMask);
            clear(myState_);
        }

        void  exceptions(io_state state) {
            exceptions((iostate)state);
        }

        fmtflags  flags() const {
            return (fmtFlag_);
        }

        fmtflags  flags(fmtflags newfmtflags) {
            fmtflags oldfmtflags = fmtFlag_;
            fmtFlag_ = (fmtflags)((int)newfmtflags & (int)FmtMask);
            return (oldfmtflags);
        }

        fmtflags  setf(fmtflags newfmtflags) {
            ios_base::fmtflags oldfmtflags = fmtFlag_;
            fmtFlag_ = (fmtflags)((int)fmtFlag_ | (int)newfmtflags & (int)FmtMask);
            return (oldfmtflags);
        }

        fmtflags  setf(fmtflags newfmtflags, fmtflags mask) {
            ios_base::fmtflags oldfmtflags = fmtFlag_;
            fmtFlag_ = (fmtflags)(((int)fmtFlag_ & (int)~mask)
                                  | ((int)newfmtflags & (int)mask & (int)FmtMask));
            return (oldfmtflags);
        }

        void  unsetf(fmtflags mask) {
            fmtFlag_ = (fmtflags)((int)fmtFlag_ & (int)~mask);
        }

        streamsize  precision() const {
            return (precision_);
        }

        streamsize  precision(streamsize newPrecision) {
            streamsize oldprecision = precision_;
            precision_ = newPrecision;
            return (oldprecision);
        }

        streamsize  width() const {
            return (wide_);
        }

        streamsize  width(streamsize newwidth) {
            streamsize oldwidth = wide_;
            wide_ = newwidth;
            return (oldwidth);
        }

        locale  getloc() const {
            return (*locale_);
        }

        locale  imbue(const locale& loc) {
            locale oldlocale = *locale_;
            *locale_ = loc;
            callFunctions_(imbue_event);
            return (oldlocale);
        }

        static int __cdecl xalloc() {
            {
                ::std::lockit lock(2);
                return (index_++);
            }
        }

        long&  iword(int index) {
            return (findArray_(index).Value);
        }

        void *&  pword(int index) {
            return (findArray_(index).Ptr);
        }

        void  register_callback(event_callback fn, int index) {
            calls_ = new(std::DebugHeapTag_func(), "..\\vc\\include\\xiosbase", 461) FuncArray_(index, fn, calls_);
        }

        ios_base&  copyfmt(const ios_base& other) {
            if (this != &other) {
                tidy_();
                *locale_ = *other.locale_;
                fmtFlag_ = other.fmtFlag_;
                precision_ = other.precision_;
                wide_ = other.wide_;
                IOSArray_ *ptr = other.array_;

                for (array_ = 0; ptr != 0; ptr = ptr->Next) {
                    if (ptr->Value != 0 || ptr->Ptr != 0) {
                        iword(ptr->Index) = ptr->Value;
                        pword(ptr->Index) = ptr->Ptr;
                    }
                }

                for (FuncArray_ *q = other.calls_; q != 0; q = q->Next) {
                    register_callback(q->Fn, q->Index);
                }

                callFunctions_(copyfmt_event);
                exceptions(other.except_);
            }

            return (*this);
        }
        virtual  ~ios_base() {
            ios_base_dtor_(this);
        }

        static bool __cdecl sync_with_stdio(bool newsync = true) {
            {
                ::std::lockit lock(2);
                const bool oldsync = sync_;
                sync_ = newsync;
                return (oldsync);
            }
        }

        static  void __cdecl Addstd(ios_base*);
        size_t stdstr_;

    protected:
        ios_base() {
        }

        void  init_() {
            locale_ = 0;
            stdstr_ = 0;
            except_ = goodbit;
            fmtFlag_ = skipws | dec;
            precision_ = 6;
            wide_ = 0;
            array_ = 0;
            calls_ = 0;
            clear(goodbit);
            locale_ = new(std::DebugHeapTag_func(), "..\\vc\\include\\xiosbase", 525) locale;
        }

    private:

        struct IOSArray_ {
        public:
            IOSArray_(int index, IOSArray_ *link)
                : Next(link)
                , Index(index)
                , Value(0)
                , Ptr(0) {
            }

            IOSArray_ *Next;
            int Index;
            long Value;
            void* Ptr;
        };
        struct FuncArray_ {
            FuncArray_(int index, event_callback newFn, FuncArray_ *link)
                : Next(link)
                , Index(index)
                , Fn(newFn) {
            }

            FuncArray_ *Next;
            int Index;
            event_callback Fn;
        };

        void  callFunctions_(event evt) {
            for (FuncArray_ *ptr = calls_; ptr != 0; ptr = ptr->Next)
                (*ptr->Fn)(evt, *this, ptr->Index);
        }

        IOSArray_&  findArray_(int index) {
            IOSArray_ *ptr, *q;

            for (ptr = array_, q = 0; ptr != 0; ptr = ptr->Next)
                if (ptr->Index == index)
                    return (*ptr);
                else if (q == 0 && ptr->Value == 0 && ptr->Ptr == 0)
                    q = ptr;

            if (q != 0) {
                q->Index = index;
                return (*q);
            }

            array_ = new(std::DebugHeapTag_func(), "..\\vc\\include\\xiosbase", 579) IOSArray_(index, array_);
            return (*array_);
        }

        void __cdecl tidy_() {
            callFunctions_(erase_event);
            IOSArray_ *q1, *q2;

            for (q1 = array_; q1 != 0; q1 = q2)
                q2 = q1->Next, std::DebugHeapDelete(q1);

            array_ = 0;
            FuncArray_ *q3, *q4;

            for (q3 = calls_; q3 != 0; q3 = q4)
                q4 = q3->Next, std::DebugHeapDelete(q3);

            calls_ = 0;
        }

        static  void __cdecl ios_base_dtor_(ios_base*);

        iostate myState_;
        iostate except_;
        fmtflags fmtFlag_;
        streamsize precision_;
        streamsize wide_;
        IOSArray_ *array_;
        FuncArray_ *calls_;
        locale* locale_;

        static int index_;
        static bool sync_;

        static  int& __cdecl index_func_();
        static  bool& __cdecl sync_func_();
    };
}


namespace std {
    template<class TElement, class TTraits>
    class basic_streambuf {
        basic_streambuf(const basic_streambuf<TElement, TTraits>&);
        basic_streambuf<TElement, TTraits>&
        operator=(const basic_streambuf<TElement, TTraits>&);
    protected:
        basic_streambuf()
            : locale_(new(std::_DebugHeapTag_func(), "..\\vc\\include\\streambuf", 27)(locale)) {
            init_();
        }

        basic_streambuf(Uninitialized) {
        }

    public:
        typedef basic_streambuf<TElement, TTraits> MyType;
        typedef TElement char_type;
        typedef TTraits traits_type;

        virtual  ~basic_streambuf() {
            std::DebugHeapDelete(locale_);
        }

        typedef typename TTraits::int_type int_type;
        typedef typename TTraits::pos_type pos_type;
        typedef typename TTraits::off_type off_type;

        pos_type pubseekoff(off_type offset, ios_base::seekdir way,
                            ios_base::openmode mode = ios_base::in | ios_base::out) {
            return (seekoff(offset, way, mode));
        }

        pos_type pubseekoff(off_type offset, ios_base::seek_dir way,
                            ios_base::open_mode mode) {
            return (pubseekoff(offset, (ios_base::seekdir)way,
                               (ios_base::openmode)mode));
        }

        pos_type pubseekpos(pos_type pos,
                            ios_base::openmode mode = ios_base::in | ios_base::out) {
            return (seekpos(pos, mode));
        }

        pos_type pubseekpos(pos_type pos, ios_base::open_mode mode) {
            return (seekpos(pos, (ios_base::openmode)mode));
        }

        MyType* pubsetbuf(TElement* buffer, streamsize count) {
            return (setbuf(buffer, count));
        }

        locale pubimbue(const locale& newlocale) {
            locale oldlocale = *locale_;
            imbue(newlocale);
            *locale_ = newlocale;
            return (oldlocale);
        }

        locale getloc() const {
            return (*locale_);
        }

        streamsize in_avail() {
            streamsize r = Gn_avail_();
            return (0 < r ? r : showmanyc());
        }

        int pubsync() {
            return (sync());
        }

        int_type sbumpc() {
            return (0 < Gn_avail_()
                    ? TTraits::to_int_type(*Gn_inc_()) : uflow());
        }

        int_type sgetc() {
            return (0 < Gn_avail_()
                    ? TTraits::to_int_type(*gptr()) : underflow());
        }

        streamsize Sgetn_s_(TElement* ptr, size_t ptr_size, streamsize count) {
            return Xsgetn_s_(ptr, ptr_size, count);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        streamsize sgetn(TElement* ptr, streamsize count) {
            return xsgetn(ptr, count);
        }

        int_type snextc() {
            return (1 < Gn_avail_()
                    ? TTraits::to_int_type(*Gn_preinc_())
                    : TTraits::eq_int_type(TTraits::eof(), sbumpc())
                    ? TTraits::eof() : sgetc());
        }

        int_type sputbackc(TElement ch) {
            return (gptr() != 0 && eback() < gptr()
                    && TTraits::eq(ch, gptr()[-1])
                    ? TTraits::to_int_type(*Gn_dec_())
                    : pbackfail(TTraits::to_int_type(ch)));
        }

        void stossc() {
            if (0 < Gn_avail_())
                Gn_inc_();
            else
                uflow();
        }

        int_type sungetc() {
            return (gptr() != 0 && eback() < gptr()
                    ? TTraits::to_int_type(*Gn_dec_()) : pbackfail());
        }

        int_type sputc(TElement ch) {
            return (0 < Pn_avail_()
                    ? TTraits::to_int_type(*Pn_inc_() = ch)
                    : overflow(TTraits::to_int_type(ch)));
        }

        streamsize sputn(const TElement* ptr, streamsize count) {
            return (xsputn(ptr, count));
        }

        void Lock_() {
            myLock_.lock();
        }

        void Unlock_() {
            myLock_.unlock();
        }
    protected:
        TElement* eback() const {
            return (*igFirst_);
        }

        TElement* gptr() const {
            return (*igNext_);
        }

        TElement* pbase() const {
            return (*ipFirst_);
        }

        TElement* pptr() const {
            return (*ipNext_);
        }

        TElement* egptr() const {
            return (*igNext_ + *igCount_);
        }

        void gbump(int offset) {
            *igCount_ -= offset;
            *igNext_ += offset;
        }

        void setg(TElement* first, TElement* next, TElement* last) {
            *igFirst_ = first;
            *igNext_ = next;
            *igCount_ = (int)(last - next);
        }

        TElement* epptr() const {
            return (*ipNext_ + *ipCount_);
        }

        TElement* Gn_dec_() {
            ++*igCount_;
            return (--*igNext_);
        }

        TElement* Gn_inc_() {
            --*igCount_;
            return ((*igNext_)++);
        }

        TElement* Gn_preinc_() {
            --*igCount_;
            return (++(*igNext_));
        }

        streamsize Gn_avail_() const {
            return (*igNext_ != 0 ? *igCount_ : 0);
        }

        void pbump(int offset) {
            *ipCount_ -= offset;
            *ipNext_ += offset;
        }

        void setp(TElement* first, TElement* last) {
            *ipFirst_ = first;
            *ipNext_ = first;
            *ipCount_ = (int)(last - first);
        }

        void setp(TElement* first, TElement* next, TElement* last) {
            *ipFirst_ = first;
            *ipNext_ = next;
            *ipCount_ = (int)(last - next);
        }

        TElement* Pn_inc_() {
            --*ipCount_;
            return ((*ipNext_)++);
        }

        streamsize Pn_avail_() const {
            return (*ipNext_ != 0 ? *ipCount_ : 0);
        }

        void init_() {
            igFirst_ = &gFirst_, ipFirst_ = &pFirst_;
            igNext_ = &gNext_, ipNext_ = &pNext_;
            igCount_ = &gCount_, ipCount_ = &pCount_;
            setp(0, 0), setg(0, 0, 0);
        }

        void init_(TElement** gFirst, TElement** gNext, int* gCount, TElement** pFirst, TElement** pNext, int* pCount) {
            igFirst_ = gFirst, ipFirst_ = pFirst;
            igNext_ = gNext, ipNext_ = pNext;
            igCount_ = gCount, ipCount_ = pCount;
        }

        virtual int_type  overflow(int_type = TTraits::eof()) {
            return (TTraits::eof());
        }

        virtual int_type  pbackfail(int_type = TTraits::eof()) {
            return (TTraits::eof());
        }

        virtual streamsize  showmanyc() {
            return (0);
        }

        virtual int_type  underflow() {
            return (TTraits::eof());
        }

        virtual int_type  uflow() {
            return (TTraits::eq_int_type(TTraits::eof(), underflow())
                    ? TTraits::eof() : TTraits::to_int_type(*Gn_inc_()));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        virtual streamsize  xsgetn(TElement* ptr, streamsize count) {
            return Xsgetn_s_(ptr, (size_t) - 1, count);
        }

        virtual streamsize  Xsgetn_s_(TElement* ptr, size_t ptrSize, streamsize count) {
            int_type meta;
            streamsize streamSize, size, copied;

            for (copied = 0; 0 < count;)
                if (0 < (streamSize = Gn_avail_())) {
                    size = streamSize;

                    if (count < size)
                        size = count;

                    _Traits_helper::copy_s<TTraits>(ptr, ptrSize, gptr(), size);
                    ptr += size;
                    copied += size;
                    count -= size;
                    gbump((int)size);
                } else if (TTraits::eq_int_type(TTraits::eof(), meta = uflow()))
                    break;
                else {
                    *ptr++ = TTraits::to_char_type(meta);
                    ++copied;
                    --count;
                }

            return (copied);
        }

        virtual streamsize  xsputn(const TElement* ptr, streamsize count) {
            streamsize streamSize, size, copied;

            for (copied = 0; 0 < count;)
                if (0 < (streamSize = Pn_avail_())) {
                    size = streamSize;

                    if (count < size)
                        size = count;

                    _Traits_helper::copy_s<TTraits>(pptr(), streamSize, ptr, size);
                    ptr += size;
                    copied += size;
                    count -= size;
                    pbump((int)size);
                } else if (TTraits::eq_int_type(TTraits::eof(), overflow(TTraits::to_int_type(*ptr))))
                    break;
                else {
                    ++ptr;
                    ++copied;
                    --count;
                }

            return (copied);
        }

        virtual pos_type  seekoff(off_type, ios_base::seekdir, ios_base::openmode = ios_base::in | ios_base::out) {
            return (streampos(_BADOFF));
        }

        virtual pos_type  seekpos(pos_type, ios_base::openmode = ios_base::in | ios_base::out) {
            return (streampos(_BADOFF));
        }

        virtual MyType* setbuf(TElement*, streamsize) {
            return (this);
        }

        virtual int  sync() {
            return (0);
        }

        virtual void  imbue(const locale&) {
        }

    private:
        Mutex myLock_;
        TElement* gFirst_;
        TElement* pFirst_;
        TElement** igFirst_;
        TElement** ipFirst_;
        TElement* gNext_;
        TElement* pNext_;
        TElement** igNext_;
        TElement** ipNext_;
        int gCount_;
        int pCount_;
        int* igCount_;
        int* ipCount_;
        locale* locale_;
    };

    template class __declspec(dllimport) basic_streambuf<char, char_traits<char> >;
    template class __declspec(dllimport) basic_streambuf<wchar_t, char_traits<wchar_t> >;
    template<class TElement, class TTraits>
    class istreambuf_iterator
            : public iterator<input_iterator_tag, TElement, typename TTraits::off_type, TElement*, TElement&> {
        typedef istreambuf_iterator<TElement, TTraits> MyType;
    public:
        typedef TElement char_type;
        typedef TTraits traits_type;
        typedef basic_streambuf<TElement, TTraits> streambuf_type;
        typedef basic_istream<TElement, TTraits> istream_type;

        typedef typename traits_type::int_type int_type;
        typedef range_checked_iterator_tag checked_iterator_category;

        istreambuf_iterator(streambuf_type* streamBuffer = 0) throw()
            : strBuffer_(streamBuffer), got_(streamBuffer == 0) {
        }

        istreambuf_iterator(istream_type& istream) throw()
            : strBuffer_(istream.rdbuf()), got_(istream.rdbuf() == 0) {
        }

        TElement operator*() const {
            if (!got_)
                ((MyType*)this)->peek_();

            if (strBuffer_ == 0)
                Debug_message(L"istreambuf_iterator is not dereferencable", L"..\\vc\\include\\streambuf", 479);

            return (value_);
        }

        MyType& operator++() {
            if (strBuffer_ == 0)
                Debug_message(L"istreambuf_iterator is not incrementable", L"..\\vc\\include\\streambuf", 490);

            inc_();
            return (*this);
        }

        MyType operator++(int) {
            if (!got_)
                peek_();

            MyType temp = *this;
            ++*this;
            return (temp);
        }

        bool equal(const MyType& right) const {
            if (!got_)
                ((MyType*)this)->peek_();

            if (!right.got_)
                ((MyType*)&right)->peek_();

            return (strBuffer_ == 0 && right.strBuffer_ == 0
                    || strBuffer_ != 0 && right.strBuffer_ != 0);
        }

    private:
        void inc_() {
            if (strBuffer_ == 0
                    || traits_type::eq_int_type(traits_type::eof(),
                                                strBuffer_->sbumpc()))
                strBuffer_ = 0, got_ = true;
            else
                got_ = false;
        }

        TElement peek_() {
            int_type meta;

            if (strBuffer_ == 0
                    || traits_type::eq_int_type(traits_type::eof(),
                                                meta = strBuffer_->sgetc()))
                strBuffer_ = 0;
            else
                value_ = traits_type::to_char_type(meta);

            got_ = true;
            return (value_);
        }

        streambuf_type* strBuffer_;
        bool got_;
        TElement value_;
    };

    template<class TElement, class TTraits> inline
    bool  operator==(
        const istreambuf_iterator<TElement, TTraits>& left,
        const istreambuf_iterator<TElement, TTraits>& right) {
        return (left.equal(right));
    }

    template<class element_type, class Traits> inline
    bool  operator!=(
        const istreambuf_iterator<element_type, Traits>& _Left,
        const istreambuf_iterator<element_type, Traits>& right) {
        return (!(_Left == right));
    }

    template<class TElement, class TTraits>
    class ostreambuf_iterator : public output_iterator {
        typedef ostreambuf_iterator<TElement, TTraits> MyType;
    public:
        typedef TElement char_type;
        typedef TTraits traits_type;
        typedef basic_streambuf<TElement, TTraits> streambuf_type;
        typedef basic_ostream<TElement, TTraits> ostream_type;
        typedef range_checked_iterator_tag checked_iterator_category;
        ostreambuf_iterator(streambuf_type* streamBuffer) throw()
            : isFailed_(false), streamBuffer_(streamBuffer) {
        }

        ostreambuf_iterator(ostream_type& ostream) throw()
            : isFailed_(false), streamBuffer_(ostream.rdbuf()) {
        }

        MyType& operator=(TElement right) {
            if (streamBuffer_ == 0
                    || traits_type::eq_int_type(TTraits::eof(),
                                                streamBuffer_->sputc(right)))
                isFailed_ = true;

            return (*this);
        }

        MyType& operator*() {
            return (*this);
        }

        MyType& operator++() {
            return (*this);
        }

        MyType& operator++(int) {
            return (*this);
        }

        bool failed() const throw() {
            return (isFailed_);
        }

    private:
        bool isFailed_;
        streambuf_type* streamBuffer_;
    };
}

namespace std {
    template<class TElement>
    class numpunct : public locale::facet {
    public:
        typedef basic_string<TElement, char_traits<TElement>, allocator<TElement> > string_type;
        typedef TElement char_type;

        static locale::id id;

        TElement  decimal_point() const {
            return (do_decimal_point());
        }

        TElement  thousands_sep() const {
            return (do_thousands_sep());
        }

        string  grouping() const {
            return (do_grouping());
        }

        string_type  falsename() const {
            return (do_falsename());
        }

        string_type  truename() const {
            return (do_truename());
        }

        explicit  numpunct(size_t refs = 0) : locale::facet(refs) {
            {
                Locinfo lobj;
                init_(lobj);

                if (kSeparator == 0)
                    kSeparator = Maklocchr(',', (TElement*)0, lobj.Getcvt());
            }
        }

        numpunct(const Locinfo& localeObj, size_t refs = 0, bool isdef = false)
            : locale::facet(refs) {
            init_(localeObj, isdef);
        }

        static size_t __cdecl Getcat(const locale::facet** facet = 0, const locale* loc = 0) {
            if (facet != 0 && *facet == 0)
                *facet = new(std::DebugHeapTag_func(), "..\\vc\\include\\xlocnum", 114) numpunct<TElement>(
                    Locinfo(loc->name()), 0, true);

            return (4);
        }

    protected:
        virtual  ~numpunct() {
            tidy_();
        }

    protected:
        numpunct(const char* locName, size_t refs = 0, bool isdef = false)
            : locale::facet(refs) {
            {
                Locinfo localeObj(locName);
                init_(localeObj, isdef);
            }
        }

        void  init_(const_Locinfo& Lobj, bool Isdef = false) {
            const lconv* ptr = Lobj.Getlconv();
            grouping_ = 0;
            falseName_ = 0;
            trueName_ = 0;

            try {
                grouping_ = Maklocstr(ptr->grouping, (char*)0, Lobj.Getcvt());
                falseName_ = Maklocstr(_Lobj._Getfalse(), (TElement*)0, Lobj.Getcvt());
                trueName_ = Maklocstr(_Lobj._Gettrue(), (TElement*)0, Lobj.Getcvt());
            } catch (...) {
                tidy_();
                throw;
            }

            Dp_ = Maklocchr(ptr->decimal_point[0], (TElement*)0, Lobj.Getcvt());
            kSeparator_ = Maklocchr(ptr->thousands_sep[0], (TElement*)0, Lobj.Getcvt());

            if (Isdef) {
                grouping_ = Maklocstr("", (char*)0, Lobj.Getcvt());
                Dp_ = Maklocchr('.', (TElement*)0, Lobj.Getcvt());
                kSeparator_ = Maklocchr(',', (TElement*)0, Lobj.Getcvt());
            }
        }

        virtual TElement  do_decimal_point() const {
            return (Dp_);
        }

        virtual TElement  do_thousands_sep() const {
            return (kSeparator_);
        }

        virtual string  do_grouping() const {
            return (string(grouping_));
        }

        virtual string_type  do_falsename() const {
            return (string_type(falseName_));
        }

        virtual string_type  do_truename() const {
            return (string_type(trueName_));
        }

    private:
        void  tidy_() {
            std::DebugHeapDelete((void*)(void*)grouping_);
            std::DebugHeapDelete((void*)(void*)falseName_);
            std::DebugHeapDelete((void*)(void*)trueName_);
        }

        const char* grouping_;
        TElement Dp_;
        TElement kSeparator_;
        const TElement* falseName_;
        const TElement* trueName_;
    };
    template<class TElement>
    class numpunct_byname : public numpunct<TElement> {
    public:
        explicit  numpunct_byname(const char* locName, size_t refs = 0)
            : numpunct<TElement>(locName, refs) {
        }

    protected:
        virtual  ~numpunct_byname() {
        }
    };
    template<class TElement>
    locale::id numpunct<TElement>::id;

    template < class TElement, class TInputIterator = istreambuf_iterator<TElement, char_traits<TElement> > >
    class num_get : public locale::facet {
    public:
        typedef numpunct<TElement> MyPunct;
        typedef basic_string<TElement, char_traits<TElement>, allocator<TElement> > MyStr;

        static size_t __cdecl Getcat(const locale::facet** facet = 0, const locale* loc = 0) {
            if (facet != 0 && *facet == 0)
                *facet = new(std::DebugHeapTag_func(), "..\\vc\\include\\xlocnum", 239) num_get<TElement, TInputIterator>(
                    Locinfo(loc->name()));

            return (4);
        }

        static locale::id id;

    protected:
        virtual  ~num_get() {
        }

    protected:
        void init_(const Locinfo& Lobj) {
            Cvt_ = Lobj.Getcvt();
        }

        Locinfo::Cvtvec Cvt_;

    public:
        explicit  num_get(size_t refs = 0)
            : locale::facet(refs) {
            {
                Locinfo Lobj;
                init_(Lobj);
            }
        }

        num_get(const Locinfo& Lobj, size_t refs = 0)
            : locale::facet(refs) {
            init_(Lobj);
        }

        typedef TElement char_type;
        typedef TInputIterator iter_type;

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase,  ios_base::iostate& state,
                            _Bool& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase,  ios_base::iostate& state,
                            unsigned short& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase,  ios_base::iostate& state,
                            unsigned int& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            long& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            unsigned long& value) const {
            return (do_get(first, last, iosBase, state, value));
        }
        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            __int64& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            unsigned __int64& value) const {
            return (do_get(first, last, iosBase, state, value));
        }
        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            float& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            double& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            long double& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

        TInputIterator  get(TInputIterator first, TInputIterator last,
                            ios_base& iosBase, ios_base::iostate& state,
                            void *& value) const {
            return (do_get(first, last, iosBase, state, value));
        }

    protected:
        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       _Bool& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 361);
            int ans = -1;

            if (iosBase.flags() & ios_base::boolalpha) {
                typedef typename MyStr::size_type MyStrSize;
                const MyPunct& punctFacet = use_facet<MyPunct>(iosBase.getloc());
                MyStr str((MyStrSize)1, (char_type)0);
                str += punctFacet.falsename();
                str += (char_type)0;
                str += punctFacet.truename();
                ans = Getloctxt(first, last, (size_t)2, str.c_str());
            } else {
                char Ac[32], *Ep;
                int errNo = 0;
                const unsigned long Ulo = ::Stoulx(Ac, &Ep,
                                                   Getifld(Ac, first, last, iosBase.flags(),
                                                           iosBase.getloc()), &errNo);

                if (Ep != Ac && errNo == 0 && Ulo <= 1)
                    ans = Ulo;
            }

            if (first == last)
                state |= ios_base::eofbit;

            if (ans < 0)
                state |= ios_base::failbit;
            else
                value = ans != 0;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       unsigned short& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 398);
            char Ac[32], *Ep;
            int Errno = 0;
            int Base = Getifld(Ac, first, last, iosBase.flags(),
                               iosBase.getloc());
            char* ptr = Ac[0] == '-' ? Ac + 1 : Ac;
            const unsigned long Ans =
                ::Stoulx(ptr, &Ep, Base, &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == ptr || Errno != 0 || 0xffff < Ans)
                state |= ios_base::failbit;
            else
                value = (unsigned short)(Ac[0] == '-'
                                         ? 0 - Ans : Ans);

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       unsigned int& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 421);
            char Ac[32], *Ep;
            int Errno = 0;
            int Base = Getifld(Ac, first, last, iosBase.flags(),
                               iosBase.getloc());
            char* ptr = Ac[0] == '-' ? Ac + 1 : Ac;
            const unsigned long Ans = ::Stoulx(ptr, &Ep, Base, &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == ptr || Errno != 0 || 0xffffffff < Ans)
                state |= ios_base::failbit;
            else
                value = Ac[0] == '-' ? 0 - Ans : Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       long& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 443);
            char Ac[32], *Ep;
            int Errno = 0;
            const long Ans = ::Stolx(Ac, &Ep,
                                     Getifld(Ac, first, last, iosBase.flags(),
                                             iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       unsigned long& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 463);
            char Ac[32], *Ep;
            int Errno = 0;
            const unsigned long Ans = ::Stoulx(Ac, &Ep,
                                               Getifld(Ac, first, last, iosBase.flags(),
                                                       iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }
        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       __int64& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 484);
            char Ac[32], *Ep;
            int Errno = 0;
            const __int64 Ans = ::Stollx(Ac, &Ep, Getifld(Ac, first, last, iosBase.flags(), iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       unsigned __int64& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 504);
            char Ac[32], *Ep;
            int Errno = 0;
            const unsigned __int64 Ans = ::Stoullx(Ac, &Ep, Getifld(Ac, first, last, iosBase.flags(), iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }
        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       float& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 525);
            char Ac[8 + 36 + 16], *Ep;
            int Errno = 0;
            const float Ans = ::Stofx(Ac, &Ep,
                                      Getffld(Ac, first, last,
                                              iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       double& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 545);
            char Ac[8 + 36 + 16], *Ep;
            int Errno = 0;
            const double Ans = ::Stodx(Ac, &Ep,
                                       Getffld(Ac, first, last,
                                               iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       long double& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 565);
            char Ac[8 + 36 + 16], *Ep;
            int Errno = 0;
            const long double Ans = ::Stoldx(Ac, &Ep,
                                             Getffld(Ac, first, last,
                                                     iosBase.getloc()), &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = Ans;

            return (first);
        }

        virtual TInputIterator  do_get(TInputIterator first, TInputIterator last,
                                       ios_base& iosBase, ios_base::iostate& state,
                                       void *& value) const {
            Debug_range(first, last, L"..\\vc\\include\\xlocnum", 585);
            char Ac[32], *Ep;
            int Errno = 0;
            int Base = Getifld(Ac, first, last, ios_base::hex,
                               iosBase.getloc());
            const unsigned __int64 Ans =
                (sizeof(void*) == sizeof(unsigned long))
                ? (unsigned __int64)::Stoulx(Ac, &Ep, Base, &Errno)
                : ::Stoullx(Ac, &Ep, Base, &Errno);

            if (first == last)
                state |= ios_base::eofbit;

            if (Ep == Ac || Errno != 0)
                state |= ios_base::failbit;
            else
                value = (void*)((char*)0 + Ans);

            return (first);
        }

    private:
        int __cdecl Getifld([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Ac,
                            TInputIterator& first, TInputIterator& last, ios_base::fmtflags basefield,
                            const locale& loc) const {
            const MyPunct& punctFacet = use_facet<MyPunct>(loc);
            const string grouping = punctFacet.grouping();
            const TElement kSeparator = grouping.size() == 0
                                        ? (TElement)0 : punctFacet.thousands_sep();
            const TElement E0 = Maklocchr('0', (TElement*)0, Cvt);
            char* ptr = Ac;

            if (first == last)
                ;
            else if (*first == Maklocchr('+', (TElement*)0, Cvt))
                *ptr++ = '+', ++first;
            else if (*first == Maklocchr('-', (TElement*)0, Cvt))
                *ptr++ = '-', ++first;

            basefield &= ios_base::basefield;
            int base = basefield == ios_base::oct ? 8
                       : basefield == ios_base::hex ? 16
                       : basefield == ios_base::Fmtzero ? 0 : 10;
            bool Seendigit = false;
            bool Nonzero = false;

            if (first != last && *first == E0) {
                Seendigit = true, ++first;

                if (first != last && (*first == Maklocchr('x', (TElement*)0, Cvt)
                                      || *first == Maklocchr('X', (TElement*)0, Cvt))
                        && (base == 0 || base == 16))
                    base = 16, Seendigit = false, ++first;
                else if (base == 0)
                    base = 8;
            }

            int Dlen = base == 0 || base == 10 ? 10
                       : base == 8 ? 8 : 16 + 6;
            string Groups((size_t)1, (char)Seendigit);
            size_t Group = 0;

            for (char* const Pe = &Ac[32 - 1];
                    first != last; ++first)
                if (::memchr((const char*)"0123456789abcdefABCDEF",
                             *ptr = Maklocbyte((TElement) * first, Cvt), Dlen) != 0) {
                    if ((Nonzero || *ptr != '0') && ptr < Pe)
                        ++ptr, Nonzero = true;

                    Seendigit = true;

                    if (Groups[Group] != 127)
                        ++Groups[Group];
                } else if (Groups[Group] == '\0'
                           || kSeparator == (TElement)0
                           || *first != kSeparator)
                    break;
                else {
                    Groups.append((string::size_type)1, '\0');
                    ++Group;
                }

            if (Group == 0)
                ;
            else if ('\0' < Groups[Group])
                ++Group;
            else
                Seendigit = false;

            for (const char* Pg = grouping.c_str(); Seendigit && 0 < Group;)
                if (*Pg == 127)
                    break;
                else if (0 < --Group && *Pg != Groups[Group]
                         || 0 == Group && *Pg < Groups[Group])
                    Seendigit = false;
                else if ('\0' < Pg[1])
                    ++Pg;

            if (Seendigit && !Nonzero)
                *ptr++ = '0';
            else if (!Seendigit)
                ptr = Ac;

            *ptr = '\0';
            return (base);
        }

        int __cdecl Getffld([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* Ac,
                            TInputIterator& first, TInputIterator& last, const locale& loc) const {
            const MyPunct& punctFacet = use_facet<MyPunct>(loc);
            const string grouping = punctFacet.grouping();
            const TElement E0 = Maklocchr('0', (TElement*)0, Cvt);
            char* ptr = Ac;
            bool Bad = false;

            if (first == last)
                ;
            else if (*first == Maklocchr('+', (TElement*)0, Cvt))
                *ptr++ = '+', ++first;
            else if (*first == Maklocchr('-', (TElement*)0, Cvt))
                *ptr++ = '-', ++first;

            bool Seendigit = false;
            int Significant = 0;
            int Pten = 0;

            if (*grouping.c_str() == 127 || *grouping.c_str() <= '\0')
                for (; first != last
                        && E0 <= *first && *first <= E0 + 9;
                        Seendigit = true, ++first)
                    if (36 <= Significant)
                        ++Pten;
                    else if (*first == E0 && Significant == 0)
                        ;
                    else {
                        *ptr++ = (char)((*first - E0) + '0');
                        ++Significant;
                    }
            else {
                const TElement kSeparator = punctFacet.thousands_sep();
                string Groups((size_t)1, '\0');
                size_t Group = 0;

                for (; first != last; ++first)
                    if (E0 <= *first && *first <= E0 + 9) {
                        Seendigit = true;

                        if (36 <= Significant)
                            ++Pten;
                        else if (*first == E0 && Significant == 0)
                            ;
                        else {
                            *ptr++ = (char)((*first - E0) + '0');
                            ++Significant;
                        }

                        if (Groups[Group] != 127)
                            ++Groups[Group];
                    } else if (Groups[Group] == '\0'
                               || kSeparator == (TElement)0
                               || *first != kSeparator)
                        break;
                    else {
                        Groups.append((size_t)1, '\0');
                        ++Group;
                    }

                if (Group == 0)
                    ;
                else if ('\0' < Groups[Group])
                    ++Group;
                else
                    Bad = true;

                for (const char* Pg = grouping.c_str();
                        !Bad && 0 < Group;)
                    if (*Pg == 127)
                        break;
                    else if (0 < --Group && *Pg != Groups[Group]
                             || 0 == Group && *Pg < Groups[Group])
                        Bad = true;
                    else if ('\0' < Pg[1])
                        ++Pg;
            }

            if (Seendigit && Significant == 0)
                *ptr++ = '0';

            if (first != last && *first == punctFacet.decimal_point())
                *ptr++ = localeconv()->decimal_point[0], ++first;

            if (Significant == 0) {
                for (; first != last && *first == E0;
                        Seendigit = true, ++first)
                    --Pten;

                if (Pten < 0)
                    *ptr++ = '0', ++Pten;
            }

            for (; first != last
                    && E0 <= *first && *first <= E0 + 9;
                    Seendigit = true, ++first)
                if (Significant < 36) {
                    *ptr++ = (char)((*first - E0) + '0');
                    ++Significant;
                }

            if (Seendigit && first != last
                    && (*first == Maklocchr('e', (TElement*)0, Cvt)
                        || *first == Maklocchr('E', (TElement*)0, Cvt))) {
                *ptr++ = 'e', ++first;
                Seendigit = false, Significant = 0;

                if (first == last)
                    ;
                else if (*first == Maklocchr('+', (TElement*)0, Cvt))
                    *ptr++ = '+', ++first;
                else if (*first == Maklocchr('-', (TElement*)0, Cvt))
                    *ptr++ = '-', ++first;

                for (; first != last && *first == E0;)
                    Seendigit = true, ++first;

                if (Seendigit)
                    *ptr++ = '0';

                for (; first != last
                        && E0 <= *first && *first <= E0 + 9;
                        Seendigit = true, ++first)
                    if (Significant < 8) {
                        *ptr++ = (char)((*first - E0) + '0');
                        ++Significant;
                    }
            }

            if (Bad || !Seendigit)
                ptr = Ac;

            *ptr = '\0';
            return (Pten);
        }

        int Hexdig(const TElement Dig, const TElement E0,
                   const TElement Al, const TElement Au) const {
            if (E0 <= Dig && Dig <= E0 + 9)
                return (Dig - E0);
            else if (Al <= Dig && Dig <= Al + 5)
                return (Dig - Al + 10);
            else if (Au <= Dig && Dig <= Au + 5)
                return (Dig - Au + 10);
            else
                return (-1);
        }

        int __cdecl Getffldx(char* Ac,
                             TInputIterator& first, TInputIterator& last,
                             ios_base& iosBase, int* Phexexp) const {
            const MyPunct& punctFacet = use_facet<MyPunct>(iosBase.getloc());
            const string grouping = punctFacet.grouping();
            const TElement E0 = Maklocchr('0', (TElement*)0, Cvt);
            const TElement Al = Maklocchr('a', (TElement*)0, Cvt);
            const TElement Au = Maklocchr('A', (TElement*)0, Cvt);
            char* ptr = Ac;
            bool Bad = false;
            int Dig;

            if (first == last)
                ;
            else if (*first == Maklocchr('+', (TElement*)0, Cvt))
                *ptr++ = '+', ++first;
            else if (*first == Maklocchr('-', (TElement*)0, Cvt))
                *ptr++ = '-', ++first;

            *ptr++ = '0';
            *ptr++ = 'x';
            bool Seendigit = false;
            int Significant = 0;
            int Phex = 0;

            if (first == last || *first != E0)
                ;
            else if (++first != last
                     && (*first == Maklocchr('x', (TElement*)0, Cvt)
                         || *first == Maklocchr('X', (TElement*)0, Cvt)))
                ++first;
            else
                Seendigit = true;

            if (*grouping.c_str() == 127 || *grouping.c_str() <= '\0')
                for (; first != last
                        && 0 <= (Dig = Hexdig(*first, E0, Al, Au));
                        Seendigit = true, ++first)
                    if (36 <= Significant)
                        ++Phex;
                    else if (*first == E0 && Significant == 0)
                        ;
                    else {
                        *ptr++ = "0123456789abcdef"[Dig];
                        ++Significant;
                    }
            else {
                const TElement kSeparator = grouping.size() == 0
                                            ? (TElement)0 : punctFacet.thousands_sep();
                string Groups((size_t)1, '\0');
                size_t Group = 0;

                for (; first != last; ++first)
                    if (0 <= (Dig = Hexdig(*first, E0, Al, Au))) {
                        Seendigit = true;

                        if (36 <= Significant)
                            ++Phex;
                        else if (*first == E0 && Significant == 0)
                            ;
                        else {
                            *ptr++ = "0123456789abcdef"[Dig];
                            ++Significant;
                        }

                        if (Groups[Group] != 127)
                            ++Groups[Group];
                    } else if (Groups[Group] == '\0'
                               || kSeparator == (TElement)0
                               || *first != kSeparator)
                        break;
                    else {
                        Groups.append((size_t)1, '\0');
                        ++Group;
                    }

                if (Group == 0)
                    ;
                else if ('\0' < Groups[Group])
                    ++Group;
                else
                    Bad = true;

                for (const char* Pg = grouping.c_str();
                        !Bad && 0 < Group;)
                    if (*Pg == 127)
                        break;
                    else if (0 < --Group && *Pg != Groups[Group]
                             || 0 == Group && *Pg < Groups[Group])
                        Bad = true;
                    else if ('\0' < Pg[1])
                        ++Pg;
            }

            if (Seendigit && Significant == 0)
                *ptr++ = '0';

            if (first != last && *first == punctFacet.decimal_point())
                *ptr++ = localeconv()->decimal_point[0], ++first;

            if (Significant == 0) {
                for (; first != last && *first == E0;
                        Seendigit = true, ++first)
                    --Phex;

                if (Phex < 0)
                    *ptr++ = '0', ++Phex;
            }

            for (; first != last
                    && 0 <= (Dig = Hexdig(*first, E0, Al, Au));
                    Seendigit = true, ++first)
                if (Significant < 36) {
                    *ptr++ = "0123456789abcdef"[Dig];
                    ++Significant;
                }

            if (Seendigit && first != last
                    && (*first == Maklocchr('p', (TElement*)0, Cvt)
                        || *first == Maklocchr('P', (TElement*)0, Cvt))) {
                *ptr++ = 'p', ++first;
                Seendigit = false, Significant = 0;

                if (first == last)
                    ;
                else if (*first == Maklocchr('+', (TElement*)0, Cvt))
                    *ptr++ = '+', ++first;
                else if (*first == Maklocchr('-', (TElement*)0, Cvt))
                    *ptr++ = '-', ++first;

                for (; first != last && *first == E0;)
                    Seendigit = true, ++first;

                if (Seendigit)
                    *ptr++ = '0';

                for (; first != last
                        && E0 <= *first && *first <= E0 + 9;
                        Seendigit = true, ++first)
                    if (Significant < 8) {
                        *ptr++ = (char)((*first - E0) + '0');
                        ++Significant;
                    }
            }

            if (Bad || !Seendigit)
                ptr = Ac;

            *ptr = '\0';
            *Phexexp = Phex;
            return (0);
        }
    };

    template<class TElement, class TInputIterator>
    locale::id num_get<TElement, TInputIterator>::id;

    template < class TElement, class TOutputIterator = ostreambuf_iterator<TElement, char_traits<TElement> > >
    class num_put : public locale::facet {
    public:
        typedef numpunct<TElement> MyPunct;
        typedef basic_string<TElement, char_traits<TElement>, allocator<TElement> > _Mystr;

        static size_t __cdecl _Getcat(const locale::facet** _Ppf = 0, const locale* _Ploc = 0) {
            if (_Ppf != 0 && *_Ppf == 0)
                *_Ppf = new(std::_DebugHeapTag_func(), "..\\vc\\include\\xlocnum", 1032) num_put<TElement, TOutputIterator>(
                    _Locinfo(_Ploc->name()));

            return (4);
        }

        static locale::id id;

    protected:
        virtual  ~num_put() {
        }

    protected:
        void  init_(const _Locinfo& _Lobj) {
            _Cvt = _Lobj._Getcvt();
        }

        _Locinfo::_Cvtvec _Cvt;

    public:
        explicit  num_put(size_t _Refs = 0)
            : locale::facet(_Refs) {
            {
                _Locinfo _Lobj;
                init_(_Lobj);
            }
        }

        num_put(const _Locinfo& _Lobj, size_t _Refs = 0)
            : locale::facet(_Refs) {
            init_(_Lobj);
        }

        typedef TElement char_type;
        typedef TOutputIterator iter_type;

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, _Bool value) const {
            return (do_put(destination, iosBase, fill, value));
        }

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, long value) const {
            return (do_put(destination, iosBase, fill, value));
        }

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, unsigned long value) const {
            return (do_put(destination, iosBase, fill, value));
        }
        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, __int64 value) const {
            return (do_put(destination, iosBase, fill, value));
        }

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, unsigned __int64 value) const {
            return (do_put(destination, iosBase, fill, value));
        }
        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, double value) const {
            return (do_put(destination, iosBase, fill, value));
        }

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, long double value) const {
            return (do_put(destination, iosBase, fill, value));
        }

        TOutputIterator  put(TOutputIterator destination,
                             ios_base& iosBase, TElement fill, const void* value) const {
            return (do_put(destination, iosBase, fill, value));
        }

    protected:
        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, _Bool value) const {
            Debug_pointer(destination, L"..\\vc\\include\\xlocnum", 1124);

            if (!(iosBase.flags() & ios_base::boolalpha))
                return (do_put(destination, iosBase, fill, (long)value));
            else {
                const MyPunct& punctFacet = use_facet<MyPunct >(iosBase.getloc());
                _Mystr str;

                if (value)
                    str.assign(punctFacet.truename());
                else
                    str.assign(punctFacet.falsename());

                size_t fillCount = iosBase.width() <= 0
                                   || (size_t)iosBase.width() <= str.size()
                                   ? 0 : (size_t)iosBase.width() - str.size();

                if ((iosBase.flags() & ios_base::adjustfield) != ios_base::left) {
                    destination = Rep(destination, fill, fillCount);
                    fillCount = 0;
                }

                destination = Put(destination, str.c_str(), str.size());
                iosBase.width(0);
                return (Rep(destination, fill, fillCount));
            }
        }

        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, long value) const {
            const size_t BufferSize = 2 * 32;
            char buffer[BufferSize], fmt[6];
            return (Iput(destination, iosBase, fill, buffer,
                         ::sprintf_s(buffer, BufferSize, Ifmt(fmt, "ld",
                                     iosBase.flags()), value)));
        }

        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, unsigned long value) const {
            const size_t BufferSize = 2 * 32;
            char buffer[BufferSize], fmt[6];
            return (Iput(destination, iosBase, fill, buffer,
                         ::sprintf_s(buffer, BufferSize, Ifmt(fmt, "lu",
                                     iosBase.flags()), value)));
        }
        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, __int64 value) const {
            const size_t BufferSize = 2 * 32;
            char buffer[BufferSize], fmt[8];
            return (Iput(destination, iosBase, fill, buffer,
                         ::sprintf_s(buffer, BufferSize, Ifmt(fmt, "Ld",
                                     iosBase.flags()), value)));
        }

        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, unsigned __int64 value) const {
            const size_t BufferSize = 2 * 32;
            char buffer[BufferSize], fmt[8];
            return (Iput(destination, iosBase, fill, buffer,
                         ::sprintf_s(buffer, BufferSize, Ifmt(fmt, "Lu",
                                     iosBase.flags()), value)));
        }
        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, double value) const {
            const size_t BufferSize = 8 + 36 + 64;
            char buffer[BufferSize], fmt[8];
            streamsize precision = iosBase.precision() <= 0
                                   && !(iosBase.flags() & ios_base::fixed)
                                   ? 6 : iosBase.precision();
            int significance = 36 < precision
                               ? 36 : (int)precision;
            precision -= significance;
            size_t beforePoint = 0;
            size_t afterPoint = 0;

            if ((iosBase.flags() & ios_base::floatfield) == ios_base::fixed
                    && value * 0.5 != value) {
                bool Signed = value < 0;

                if (Signed)
                    value = -value;

                for (; 1e35 <= value && beforePoint < 5000; beforePoint += 10)
                    value /= 1e10;

                if (0 < value)
                    for (; 10 <= precision && value <= 1e-35
                            && afterPoint < 5000; afterPoint += 10) {
                        value *= 1e10;
                        precision -= 10;
                    }

                if (Signed)
                    value = -value;
            }

            return (Fput(destination, iosBase, fill, buffer,
                         beforePoint, afterPoint, precision,
                         ::sprintf_s(buffer, BufferSize, Ffmt(fmt, 0, iosBase.flags()),
                                     significance, value)));
        }

        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, long double value) const {
            const size_t BufferSize = 8 + 36 + 64;
            char buffer[BufferSize], fmt[8];
            streamsize precision = iosBase.precision() <= 0
                                   && !(iosBase.flags() & ios_base::fixed)
                                   ? 6 : iosBase.precision();
            int significance = 36 < precision
                               ? 36 : (int)precision;
            precision -= significance;
            size_t beforePoint = 0;
            size_t afterPoint = 0;

            if ((iosBase.flags() & ios_base::floatfield) == ios_base::fixed) {
                bool Signed = value < 0;

                if (Signed)
                    value = -value;

                for (; 1e35 <= value && beforePoint < 5000; beforePoint += 10)
                    value /= 1e10;

                if (0 < value)
                    for (; 10 <= precision && value <= 1e-35
                            && afterPoint < 5000; afterPoint += 10) {
                        value *= 1e10;
                        precision -= 10;
                    }

                if (Signed)
                    value = -value;
            }

            return (Fput(destination, iosBase, fill, buffer,
                         beforePoint, afterPoint, precision,
                         ::sprintf_s(buffer, BufferSize, Ffmt(fmt, 'L', iosBase.flags()),
                                     significance, value)));
        }

        virtual TOutputIterator  do_put(TOutputIterator destination,
                                        ios_base& iosBase, TElement fill, const void* value) const {
            const size_t BufferSize = 2 * 32;
            char buffer[BufferSize];
            return (Iput(destination, iosBase, fill, buffer,
                         ::sprintf_s(buffer, BufferSize, "%p", value)));
        }

    private:
        char* __cdecl Ffmt([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* fmt,
                           char spec, ios_base::fmtflags flags) const {
            char* ptr = fmt;
            *ptr++ = '%';

            if (flags & ios_base::showpos)
                *ptr++ = '+';

            if (flags & ios_base::showpoint)
                *ptr++ = '#';

            *ptr++ = '.';
            *ptr++ = '*';

            if (spec != '\0')
                *ptr++ = spec;

            ios_base::fmtflags fmtFlag = flags & ios_base::floatfield;
            *ptr++ = fmtFlag == ios_base::fixed ? 'f'
                     : fmtFlag == ios_base::hexfloat ? 'a'
                     : fmtFlag == ios_base::scientific ? 'e' : 'g';
            *ptr = '\0';
            return (fmt);
        }

        TOutputIterator __cdecl Fput(TOutputIterator destination,
                                     ios_base& iosBase, TElement fill, const char* buffer,
                                     size_t beforePoint, size_t afterPoint,
                                     size_t trailing, size_t count) const {
            Debug_pointer(destination, L"..\\vc\\include\\xlocnum", 1314);
            const MyPunct& punctFacet = use_facet<MyPunct >(iosBase.getloc());
            const string grouping_ = punctFacet.grouping();
            const TElement kSeparator = punctFacet.thousands_sep();
            string groupString;
            const TElement E0 = _Maklocchr('0', (TElement*)0, _Cvt);
            size_t prefix = buffer[0] == '+' || buffer[0] == '-' ? 1 : 0;
            char enders[3];
            enders[0] = ::localeconv()->decimal_point[0];
            enders[1] = 'e';
            enders[2] = '\0';
            const char* Eptr = (const char*)::memchr(buffer,
                               'e', count);
            const char* pointPtr = (const char*)::memchr(buffer,
                                   enders[0], count);

            if (pointPtr == 0)
                trailing = 0;

            if (*grouping_.c_str() != 127 && '\0' < *grouping_.c_str()) {
                groupString.append(buffer, count);

                if (Eptr == 0)
                    groupString.append(trailing, '0');
                else {
                    if (pointPtr == 0) {
                        groupString.append(beforePoint, '0');
                        beforePoint = 0;
                    }

                    groupString.insert(Eptr - buffer, trailing, '0');
                }

                trailing = 0;

                if (pointPtr == 0)
                    groupString.append(beforePoint, '0');
                else {
                    groupString.insert(pointPtr - buffer + 1, afterPoint, '0');
                    groupString.insert(pointPtr - buffer, beforePoint, '0');
                    afterPoint = 0;
                }

                beforePoint = 0;
                const char* pGroup = grouping_.c_str();
                size_t offset = ::strcspn(&groupString[0], &enders[0]);

                while (*pGroup != 127 && '\0' < *pGroup
                        && (size_t)*pGroup < offset - prefix) {
                    groupString.insert(offset -= *pGroup, (size_t)1, '\0');

                    if ('\0' < pGroup[1])
                        ++pGroup;
                }

                buffer = &groupString[0];
                trailing = 0;
                count = groupString.size();
            }

            size_t fillCount = beforePoint + afterPoint + trailing + count;
            fillCount = iosBase.width() <= 0
                        || (size_t)iosBase.width() <= fillCount
                        ? 0 : (size_t)iosBase.width() - fillCount;
            ios_base::fmtflags adjustField =
                iosBase.flags() & ios_base::adjustfield;

            if (adjustField != ios_base::left
                    && adjustField != ios_base::internal) {
                destination = Rep(destination, fill, fillCount);
                fillCount = 0;
            } else if (adjustField == ios_base::internal) {
                if (0 < prefix) {
                    destination = Putc(destination, buffer, 1);
                    ++buffer, --count;
                }

                destination = Rep(destination, fill, fillCount);
                fillCount = 0;
            }

            pointPtr = (const char*)::memchr(buffer,
                                             enders[0], count);

            if (pointPtr != 0) {
                size_t fracOffset = pointPtr - buffer + 1;
                destination = Putgrouped(destination, buffer, fracOffset - 1, kSeparator);
                destination = Rep(destination, E0, beforePoint);
                destination = Rep(destination, punctFacet.decimal_point(), 1);
                destination = Rep(destination, E0, afterPoint);
                buffer += fracOffset, count -= fracOffset;
            }

            Eptr = (const char*)::memchr(buffer,
                                         'e', count);

            if (Eptr != 0) {
                size_t expOffset = Eptr - buffer + 1;
                destination = Putgrouped(destination, buffer, expOffset - 1, kSeparator);
                destination = Rep(destination, E0, trailing), trailing = 0;
                destination = Putc(destination, iosBase.flags() & ios_base::uppercase
                                   ? "E" : "e", 1);
                buffer += expOffset, count -= expOffset;
            }

            destination = Putgrouped(destination, buffer, count,
                                     kSeparator);
            destination = Rep(destination, E0, trailing);
            iosBase.width(0);
            return (Rep(destination, fill, fillCount));
        }

        char* __cdecl Ifmt([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* fmt,
                           const char* spec, ios_base::fmtflags flags) const {
            char* ptr = fmt;
            *ptr++ = '%';

            if (flags & ios_base::showpos)
                *ptr++ = '+';

            if (flags & ios_base::showbase)
                *ptr++ = '#';

            if (spec[0] != 'L')
                *ptr++ = spec[0];
            else {
                *ptr++ = 'I';
                *ptr++ = '6';
                *ptr++ = '4';
            }

            ios_base::fmtflags baseField = flags & ios_base::basefield;
            *ptr++ = baseField == ios_base::oct ? 'o'
                     : baseField != ios_base::hex ? spec[1]
                     : flags & ios_base::uppercase ? 'X' : 'x';
            *ptr = '\0';
            return (fmt);
        }

        TOutputIterator __cdecl Iput(TOutputIterator destination, ios_base& iosBase, TElement fill, [SA_Pre(Null=SA_No, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Post(Deref=1, Valid=SA_Yes)] char* buffer, size_t count) const {
            Debug_pointer(destination, L"..\\vc\\include\\xlocnum", 1459);
            const MyPunct& punctFacet = use_facet<MyPunct >(iosBase.getloc());
            const string grouping_ = punctFacet.grouping();
            const size_t prefix = *buffer == '+' || *buffer == '-' ? 1
                                  : *buffer == '0' && (buffer[1] == 'x' || buffer[1] == 'X') ? 2
                                  : 0;

            if (*grouping_.c_str() != 127 && '\0' < *grouping_.c_str()) {
                const char* pGroup = grouping_.c_str();
                size_t offset = count;

                while (*pGroup != 127 && '\0' < *pGroup
                        && (size_t)*pGroup < offset - prefix) {
                    offset -= *pGroup;
                    ::memmove_s((&buffer[offset + 1]), (count + 1 - offset), (&buffer[offset]), (count + 1 - offset));
                    buffer[offset] = '\0', ++count;

                    if ('\0' < pGroup[1])
                        ++pGroup;
                }
            }

            size_t fillCount = iosBase.width() <= 0
                               || (size_t)iosBase.width() <= count
                               ? 0 : (size_t)iosBase.width() - count;
            ios_base::fmtflags adjustField = iosBase.flags() & ios_base::adjustfield;

            if (adjustField != ios_base::left && adjustField != ios_base::internal) {
                destination = Rep(destination, fill, fillCount);
                fillCount = 0;
            } else if (adjustField == ios_base::internal) {
                destination = Putc(destination, buffer, prefix);
                buffer += prefix, count -= prefix;
                destination = Rep(destination, fill, fillCount), fillCount = 0;
            }

            destination = Putgrouped(destination, buffer, count, punctFacet.thousands_sep());
            iosBase.width(0);
            return (Rep(destination, fill, fillCount));
        }

        TOutputIterator __cdecl Put(TOutputIterator destination, const TElement* ptr, size_t count) const {
            for (; 0 < count; --count, ++destination, ++ptr)
                *destination = *ptr;

            return (destination);
        }

        TOutputIterator __cdecl Putc(TOutputIterator destination, const char* ptr, size_t count) const {
            for (; 0 < count; --count, ++destination, ++ptr)
                *destination = _Maklocchr(*ptr, (TElement*)0, _Cvt);

            return (destination);
        }

        TOutputIterator __cdecl Putgrouped(TOutputIterator destination, const char* ptr, size_t count, TElement kSeparator) const {
            for (; ; ++ptr, --count) {
                const char* _Pend =
                    (const char*)::memchr(ptr, '\0', count);
                size_t _Groupsize = _Pend != 0 ? _Pend - ptr : count;
                destination = Putc(destination, ptr, _Groupsize);
                ptr += _Groupsize, count -= _Groupsize;

                if (count == 0)
                    break;

                if (kSeparator != (TElement)0)
                    destination = Rep(destination, kSeparator, 1);
            }

            return (destination);
        }

        TOutputIterator __cdecl Rep(TOutputIterator destination, TElement chr, size_t count) const {
            for (; 0 < count; --count, ++destination)
                *destination = chr;

            return (destination);
        }
    };
    template<class TElement, class TOutputIterator>
    locale::id num_put<TElement, TOutputIterator>::id;

    template class __declspec(dllimport) numpunct<char>;
    template class __declspec(dllimport) num_get<char, istreambuf_iterator<char, char_traits<char> > >;
    template class __declspec(dllimport) num_put<char, ostreambuf_iterator<char, char_traits<char> > >;

    template class __declspec(dllimport) numpunct<wchar_t>;
    template class __declspec(dllimport) num_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >;
    template class __declspec(dllimport) num_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >;

#pragma warning(pop)
}

namespace std {
    template<class element_type, class Traits>
    class basic_ios : public ios_base {
    public:
        typedef basic_ios<element_type, Traits> MyType;
        typedef basic_ostream<element_type, Traits> MyOutputStreamType;
        typedef basic_streambuf<element_type, Traits> MyStreamBufferType;
        typedef ctype<element_type> Ctype;
        typedef element_type char_type;
        typedef Traits traits_type;
        typedef typename Traits::int_type int_type;
        typedef typename Traits::pos_type pos_type;
        typedef typename Traits::off_type off_type;

        explicit  basic_ios(MyStreamBufferType* streamBuffer) {
            init(streamBuffer);
        }

        virtual  ~basic_ios() {
        }

        void  clear(iostate state = goodbit, bool reRaise = false) {
            ios_base::clear((iostate)(myStreamBuffer_ == 0
                                      ? (int)state | (int)badbit : (int)state), reRaise);
        }

        void  clear(io_state state) {
            clear((iostate)state);
        }

        void  setstate(iostate state, bool reRaise = false) {
            if (state != goodbit)
                clear((iostate)((int)rdstate() | (int)state), reRaise);
        }

        void  setstate(io_state state) {
            setstate((iostate)state);
        }

        MyType&  copyfmt(const MyType& right) {
            tieStream_ = right.tie();
            fillChar_ = right.fill();
            ios_base::copyfmt(right);
            return (*this);
        }

        MyOutputStreamType* tie() const {
            return (tieStream_);
        }

        MyOutputStreamType* tie(MyOutputStreamType* newTie) {
            MyOutputStreamType* oldTie = tieStream_;
            tieStream_ = newTie;
            return (oldTie);
        }

        MyStreamBufferType* rdbuf() const {
            return (myStreamBuffer_);
        }

        MyStreamBufferType* rdbuf(MyStreamBufferType* streamBuffer) {
            MyStreamBufferType* oldStreamBuffer = myStreamBuffer_;
            myStreamBuffer_ = streamBuffer;
            clear();
            return (oldStreamBuffer);
        }

        locale  imbue(const locale& loc) {
            locale oldLocale = ios_base::imbue(loc);

            if (rdbuf() != 0)
                rdbuf()->pubimbue(loc);

            return (oldLocale);
        }

        element_type  fill() const {
            return (fillChar_);
        }

        element_type  fill(element_type newFill) {
            element_type _Oldfill = fillChar_;
            fillChar_ = newFill;
            return (_Oldfill);
        }

        char  narrow(element_type chr, char dflt = '\0') const {
            const Ctype& ctypeFacet = use_facet<Ctype >(getloc());
            return (ctypeFacet.narrow(chr, dflt));
        }

        element_type  widen(char _Byte) const {
            const Ctype& ctypeFacet = use_facet<Ctype >(getloc());
            return (ctypeFacet.widen(_Byte));
        }

    protected:
        void  init(MyStreamBufferType* streamBuffer = 0, bool isStd = false) {
            init_();
            myStreamBuffer_ = streamBuffer;
            tieStream_ = 0;
            fillChar_ = widen(' ');

            if (myStreamBuffer_ == 0)
                setstate(badbit);

            if (isStd)
                _Addstd(this);
            else
                _Stdstr = 0;
        }

        basic_ios() {
        }

    private:
        basic_ios(const MyType&);
        MyType&  operator=(const MyType&);

        MyStreamBufferType* myStreamBuffer_;
        MyOutputStreamType* tieStream_;
        element_type fillChar_;
    };

    template class __declspec(dllimport) basic_ios<char, char_traits<char> >;
    template class __declspec(dllimport) basic_ios<wchar_t, char_traits<wchar_t> >;
    inline ios_base& __cdecl boolalpha(ios_base& iosbase) {
        iosbase.setf(ios_base::boolalpha);
        return (iosbase);
    }

    inline ios_base& __cdecl dec(ios_base& iosbase) {
        iosbase.setf(ios_base::dec, ios_base::basefield);
        return (iosbase);
    }

    inline ios_base& __cdecl fixed(ios_base& iosbase) {
        iosbase.setf(ios_base::fixed, ios_base::floatfield);
        return (iosbase);
    }

    inline ios_base& __cdecl hex(ios_base& iosbase) {
        iosbase.setf(ios_base::hex, ios_base::basefield);
        return (iosbase);
    }

    namespace tr1 {
        inline std::ios_base& __cdecl hexfloat(std::ios_base& iosbase) {
            iosbase.setf(std::ios_base::hexfloat, std::ios_base::floatfield);
            return (iosbase);
        }
    }
    using tr1::hexfloat;

    inline ios_base& __cdecl internal(ios_base& iosbase) {
        iosbase.setf(ios_base::internal, ios_base::adjustfield);
        return (iosbase);
    }

    inline ios_base& __cdecl left(ios_base& iosbase) {
        iosbase.setf(ios_base::left, ios_base::adjustfield);
        return (iosbase);
    }

    inline ios_base& __cdecl noboolalpha(ios_base& iosbase) {
        iosbase.unsetf(ios_base::boolalpha);
        return (iosbase);
    }

    inline ios_base& __cdecl noshowbase(ios_base& iosbase) {
        iosbase.unsetf(ios_base::showbase);
        return (iosbase);
    }

    inline ios_base& __cdecl noshowpoint(ios_base& iosbase) {
        iosbase.unsetf(ios_base::showpoint);
        return (iosbase);
    }

    inline ios_base& __cdecl noshowpos(ios_base& iosbase) {
        iosbase.unsetf(ios_base::showpos);
        return (iosbase);
    }

    inline ios_base& __cdecl noskipws(ios_base& iosbase) {
        iosbase.unsetf(ios_base::skipws);
        return (iosbase);
    }

    inline ios_base& __cdecl nounitbuf(ios_base& iosbase) {
        iosbase.unsetf(ios_base::unitbuf);
        return (iosbase);
    }

    inline ios_base& __cdecl nouppercase(ios_base& iosbase) {
        iosbase.unsetf(ios_base::uppercase);
        return (iosbase);
    }

    inline ios_base& __cdecl oct(ios_base& iosbase) {
        iosbase.setf(ios_base::oct, ios_base::basefield);
        return (iosbase);
    }

    inline ios_base& __cdecl right(ios_base& iosbase) {
        iosbase.setf(ios_base::right, ios_base::adjustfield);
        return (iosbase);
    }

    inline ios_base& __cdecl scientific(ios_base& iosbase) {
        iosbase.setf(ios_base::scientific, ios_base::floatfield);
        return (iosbase);
    }

    inline ios_base& __cdecl showbase(ios_base& iosbase) {
        iosbase.setf(ios_base::showbase);
        return (iosbase);
    }

    inline ios_base& __cdecl showpoint(ios_base& iosbase) {
        iosbase.setf(ios_base::showpoint);
        return (iosbase);
    }

    inline ios_base& __cdecl showpos(ios_base& iosbase) {
        iosbase.setf(ios_base::showpos);
        return (iosbase);
    }

    inline ios_base& __cdecl skipws(ios_base& iosbase) {
        iosbase.setf(ios_base::skipws);
        return (iosbase);
    }

    inline ios_base& __cdecl unitbuf(ios_base& iosbase) {
        iosbase.setf(ios_base::unitbuf);
        return (iosbase);
    }

    inline ios_base& __cdecl uppercase(ios_base& iosbase) {
        iosbase.setf(ios_base::uppercase);
        return (iosbase);
    }
}

namespace std {
    template<class element_type, class Traits>
    class basic_ostream : virtual public basic_ios<element_type, Traits> {
    public:
        typedef basic_ostream<element_type, Traits> MyType;
        typedef basic_ios<element_type, Traits> MyIOSType;
        typedef basic_streambuf<element_type, Traits> MyStreamBufferType;
        typedef ostreambuf_iterator<element_type, Traits> IteratorType;
        typedef num_put<element_type, IteratorType> NumPut;

        explicit  basic_ostream(basic_streambuf<element_type, Traits> *streamBuffer, bool isStd = false) {
            MyIOSType::init(streamBuffer, isStd);
        }

        basic_ostream(Uninitialized, bool addIt = true) {
            if (addIt)
                ios_base::_Addstd(this);
        }

        virtual  ~basic_ostream() {
        }

        typedef typename Traits::int_type int_type;
        typedef typename Traits::pos_type pos_type;
        typedef typename Traits::off_type off_type;

        class Sentry_base {
        public:
            Sentry_base(MyType& outputStream)
                : MyOutputStream_(outputStream) {
                if (MyOutputStream_.rdbuf() != 0)
                    MyOutputStream_.rdbuf()->Lock();
            }

            ~Sentry_base() {
                if (MyOutputStream_.rdbuf() != 0)
                    MyOutputStream_.rdbuf()->Unlock();
            }

            MyType& MyOutputStream_;
        };

        class sentry : public Sentry_base {
        public:
            explicit  sentry(MyType& outputStream) : Sentry_base(outputStream) {
                if (outputStream.good() && outputStream.tie() != 0)
                    outputStream.tie()->flush();

                ok = outputStream.good();
            }

            ~sentry() {
                if (!std:: uncaught_exception())
                    this->MyOutputStream_.Osfx();
            }
            operator bool() const {
                return (ok);
            }

        private:
            sentry(const sentry&);
            sentry&  operator=(const sentry&);

            bool ok;
        };

        bool  opfx() {
            if (ios_base::good() && MyIOSType::tie() != 0)
                MyIOSType::tie()->flush();

            return (ios_base::good());
        }

        void  osfx() {
            Osfx();
        }

        void  Osfx() {
            try {
                if (ios_base::flags() & ios_base::unitbuf)
                    flush();
            } catch (...) {
            }
        }
        MyType&  operator<<(MyType& (__cdecl* Pfn)(MyType&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\ostream", 172);
            return ((*Pfn)(*this));
        }

        MyType&  operator<<(MyIOSType& (__cdecl* Pfn)(MyIOSType&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\ostream", 178);
            (*Pfn)(*(MyIOSType*)this);
            return (*this);
        }

        MyType&  operator<<(ios_base& (__cdecl* Pfn)(ios_base&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\ostream", 185);
            (*Pfn)(*(ios_base*)this);
            return (*this);
        }

        MyType&  operator<<(_Bool val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(short val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());
                ios_base::fmtflags baseFlag =
                    ios_base::flags() & ios_base::basefield;
                long temp = (baseFlag == ios_base::oct
                             || baseFlag == ios_base::hex)
                            ? (long)(unsigned short)val : (long)val;

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), temp).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(unsigned short val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), (unsigned long)val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(int __w64 val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());
                ios_base::fmtflags baseFlag =
                    ios_base::flags() & ios_base::basefield;
                long temp = (baseFlag == ios_base::oct
                             || baseFlag == ios_base::hex)
                            ? (long)(unsigned int)val : (long)val;

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), temp).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(unsigned int __w64 val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), (unsigned long)val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(long val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(unsigned long __w64 val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), (unsigned long)val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }
        MyType&  operator<<(__int64 val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(unsigned __int64 val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }
        MyType&  operator<<(float val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), (double)val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(double val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(long double val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(const void* val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const NumPut& numput_facet = use_facet<NumPut >(ios_base::getloc());

                try {
                    if (numput_facet.put(IteratorType(MyIOSType::rdbuf()), *this,
                                         MyIOSType::fill(), val).failed())
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator<<(MyStreamBufferType* streamBuffer) {
            ios_base::iostate state = ios_base::goodbit;
            bool copied = false;
            const sentry ok(*this);

            if (ok && streamBuffer != 0)
                for (int_type meta = Traits::eof(); ; copied = true) {
                    try {
                        meta = Traits::eq_int_type(Traits::eof(), meta)
                               ? streamBuffer->sgetc() : streamBuffer->snextc();
                    } catch (...) {
                        MyIOSType::setstate(ios_base::failbit);
                        throw;
                    }

                    if (Traits::eq_int_type(Traits::eof(), meta))
                        break;

                    try {
                        if (Traits::eq_int_type(Traits::eof(),
                                                MyIOSType::rdbuf()->sputc(
                                                    Traits::to_char_type(meta)))) {
                            state |= ios_base::badbit;
                            break;
                        }
                    } catch (...) {
                        MyIOSType::setstate(ios_base::badbit, true);
                    }
                }

            ios_base::width(0);
            MyIOSType::setstate(streamBuffer == 0 ? ios_base::badbit
                                : !copied ? state | ios_base::failbit : state);
            return (*this);
        }

        MyType&  put(element_type chr) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (!ok)
                state |= ios_base::badbit;
            else {
                try {
                    if (Traits::eq_int_type(Traits::eof(),
                                            MyIOSType::rdbuf()->sputc(chr)))
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  write(const element_type* str,
                       streamsize count) {
            if (0 < count)
                Debug_pointer(str, L"..\\vc\\include\\ostream", 531);

            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (!ok)
                state |= ios_base::badbit;
            else {
                try {
                    if (MyIOSType::rdbuf()->sputn(str, count) != count)
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  flush() {
            ios_base::iostate state = ios_base::goodbit;

            if (!ios_base::fail() && MyIOSType::rdbuf()->pubsync() == -1)
                state |= ios_base::badbit;

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  seekp(pos_type position) {
            if (!ios_base::fail()
                    && (off_type)MyIOSType::rdbuf()->pubseekpos(position,
                            ios_base::out) == _BADOFF)
                MyIOSType::setstate(ios_base::failbit);

            return (*this);
        }

        MyType&  seekp(off_type offset, ios_base::seekdir way) {
            if (!ios_base::fail()
                    && (off_type)MyIOSType::rdbuf()->pubseekoff(offset, way,
                            ios_base::out) == _BADOFF)
                MyIOSType::setstate(ios_base::failbit);

            return (*this);
        }

        pos_type  tellp() {
            if (!ios_base::fail())
                return (MyIOSType::rdbuf()->pubseekoff(0,
                                                       ios_base::cur, ios_base::out));
            else
                return (pos_type(_BADOFF));
        }
    };

    template class __declspec(dllimport) basic_ostream<char, char_traits<char> >;
    template class __declspec(dllimport) basic_ostream<wchar_t, char_traits<wchar_t> >;

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>& __cdecl operator<<(basic_ostream<element_type, Traits>& outputStream, const char* val) {
        ios_base::iostate state = ios_base::goodbit;
        streamsize count = (streamsize)::strlen(val);
        streamsize pad = outputStream.width() <= 0 || outputStream.width() <= count
                         ? 0 : outputStream.width() - count;
        const typename basic_ostream<element_type, Traits>::sentry ok(outputStream);

        if (!ok)
            state |= ios_base::badbit;
        else {
            try {
                const ctype<element_type>& ctypeFacet = use_facet<ctype<element_type> >(outputStream.getloc());

                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                for (; state == ios_base::goodbit && 0 < count; --count, ++val)
                    if (Traits::eq_int_type(Traits::eof(),
                                            outputStream.rdbuf()->sputc(ctypeFacet.widen(*val))))
                        state |= ios_base::badbit;

                if (state == ios_base::goodbit)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                outputStream.width(0);
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.setstate(state);
        return (outputStream);
    }

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>& __cdecl operator<<(
        basic_ostream<element_type, Traits>& outputStream, char chr) {
        ios_base::iostate state = ios_base::goodbit;
        const typename basic_ostream<element_type, Traits>::sentry ok(outputStream);

        if (ok) {
            const ctype<element_type>& ctypeFacet = use_facet<ctype<element_type> >(outputStream.getloc());
            streamsize pad = outputStream.width() <= 1 ? 0 : outputStream.width() - 1;

            try {
                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; state == ios_base::goodbit && 0 < pad;
                            --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill())))
                            state |= ios_base::badbit;

                if (state == ios_base::goodbit
                        && Traits::eq_int_type(Traits::eof(),
                                               outputStream.rdbuf()->sputc(ctypeFacet.widen(chr))))
                    state |= ios_base::badbit;

                for (; state == ios_base::goodbit && 0 < pad;
                        --pad)
                    if (Traits::eq_int_type(Traits::eof(),
                                            outputStream.rdbuf()->sputc(outputStream.fill())))
                        state |= ios_base::badbit;
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.width(0);
        outputStream.setstate(state);
        return (outputStream);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream,
        const char* val) {
        typedef char element_type;
        typedef basic_ostream<element_type, Traits> MyOutputStreamType;
        ios_base::iostate state = ios_base::goodbit;
        streamsize count = (streamsize)Traits::length(val);
        streamsize pad = outputStream.width() <= 0 || outputStream.width() <= count
                         ? 0 : outputStream.width() - count;
        const typename MyOutputStreamType::sentry ok(outputStream);

        if (!ok)
            state |= ios_base::badbit;
        else {
            try {
                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                if (state == ios_base::goodbit
                        && outputStream.rdbuf()->sputn(val, count) != count)
                    state |= ios_base::badbit;

                if (state == ios_base::goodbit)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                outputStream.width(0);
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.setstate(state);
        return (outputStream);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream, char chr) {
        typedef char element_type;
        typedef basic_ostream<element_type, Traits> MyOutputStreamType;
        ios_base::iostate state = ios_base::goodbit;
        const typename MyOutputStreamType::sentry ok(outputStream);

        if (ok) {
            streamsize pad = outputStream.width() <= 1 ? 0 : outputStream.width() - 1;

            try {
                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; state == ios_base::goodbit && 0 < pad;
                            --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill())))
                            state |= ios_base::badbit;

                if (state == ios_base::goodbit
                        && Traits::eq_int_type(Traits::eof(),
                                               outputStream.rdbuf()->sputc(chr)))
                    state |= ios_base::badbit;

                for (; state == ios_base::goodbit && 0 < pad;
                        --pad)
                    if (Traits::eq_int_type(Traits::eof(),
                                            outputStream.rdbuf()->sputc(outputStream.fill())))
                        state |= ios_base::badbit;
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.width(0);
        outputStream.setstate(state);
        return (outputStream);
    }

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>& __cdecl operator<<(
        basic_ostream<element_type, Traits>& outputStream, const element_type* val) {
        typedef basic_ostream<element_type, Traits> MyOutputStreamType;
        ios_base::iostate state = ios_base::goodbit;
        streamsize count = (streamsize)Traits::length(val);
        streamsize pad = outputStream.width() <= 0 || outputStream.width() <= count
                         ? 0 : outputStream.width() - count;
        const typename MyOutputStreamType::sentry ok(outputStream);

        if (!ok)
            state |= ios_base::badbit;
        else {
            try {
                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                if (state == ios_base::goodbit
                        && outputStream.rdbuf()->sputn(val, count) != count)
                    state |= ios_base::badbit;

                if (state == ios_base::goodbit)
                    for (; 0 < pad; --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill()))) {
                            state |= ios_base::badbit;
                            break;
                        }

                outputStream.width(0);
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.setstate(state);
        return (outputStream);
    }

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>& __cdecl operator<<(
        basic_ostream<element_type, Traits>& outputStream, element_type chr) {
        typedef basic_ostream<element_type, Traits> MyOutputStreamType;
        ios_base::iostate state = ios_base::goodbit;
        const typename MyOutputStreamType::sentry ok(outputStream);

        if (ok) {
            streamsize pad = outputStream.width() <= 1 ? 0 : outputStream.width() - 1;

            try {
                if ((outputStream.flags() & ios_base::adjustfield) != ios_base::left)
                    for (; state == ios_base::goodbit && 0 < pad;
                            --pad)
                        if (Traits::eq_int_type(Traits::eof(),
                                                outputStream.rdbuf()->sputc(outputStream.fill())))
                            state |= ios_base::badbit;

                if (state == ios_base::goodbit
                        && Traits::eq_int_type(Traits::eof(),
                                               outputStream.rdbuf()->sputc(chr)))
                    state |= ios_base::badbit;

                for (; state == ios_base::goodbit && 0 < pad;
                        --pad)
                    if (Traits::eq_int_type(Traits::eof(),
                                            outputStream.rdbuf()->sputc(outputStream.fill())))
                        state |= ios_base::badbit;
            } catch (...) {
                (outputStream).setstate(ios_base::badbit, true);
            }
        }

        outputStream.width(0);
        outputStream.setstate(state);
        return (outputStream);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream, const signed char* val) {
        return (outputStream << (const char*)val);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream, signed char chr) {
        return (outputStream << (char)chr);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream, const unsigned char* val) {
        return (outputStream << (const char*)val);
    }

    template<class Traits> inline
    basic_ostream<char, Traits>& __cdecl operator<<(
        basic_ostream<char, Traits>& outputStream, unsigned char chr) {
        return (outputStream << (char)chr);
    }
    template < class element_type,
             class Traits > inline
    basic_ostream<element_type, Traits>&
    __cdecl endl(basic_ostream<element_type, Traits>& outputStream) {
        outputStream.put(outputStream.widen('\n'));
        outputStream.flush();
        return (outputStream);
    }

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>&
    __cdecl ends(basic_ostream<element_type, Traits>& outputStream) {
        outputStream.put(element_type());
        return (outputStream);
    }

    template<class element_type, class Traits> inline
    basic_ostream<element_type, Traits>&
    __cdecl flush(basic_ostream<element_type, Traits>& outputStream) {
        outputStream.flush();
        return (outputStream);
    }

    __declspec(dllimport) inline basic_ostream<char, char_traits<char> >&
    __cdecl endl(basic_ostream<char, char_traits<char> >& outputStream) {
        outputStream.put('\n');
        outputStream.flush();
        return (outputStream);
    }

    __declspec(dllimport) inline basic_ostream<wchar_t, char_traits<wchar_t> >&
    __cdecl endl(basic_ostream<wchar_t, char_traits<wchar_t> >& outputStream) {
        outputStream.put('\n');
        outputStream.flush();
        return (outputStream);
    }
    __declspec(dllimport) inline basic_ostream<char, char_traits<char> >&
    __cdecl ends(basic_ostream<char, char_traits<char> >& outputStream) {
        outputStream.put('\0');
        return (outputStream);
    }

    __declspec(dllimport) inline basic_ostream<wchar_t, char_traits<wchar_t> >&
    __cdecl ends(basic_ostream < wchar_t,
                 char_traits<wchar_t> > & outputStream) {
        outputStream.put('\0');
        return (outputStream);
    }
    __declspec(dllimport) inline basic_ostream<char, char_traits<char> >&
    __cdecl flush(basic_ostream<char, char_traits<char> >& outputStream) {
        outputStream.flush();
        return (outputStream);
    }

    __declspec(dllimport) inline basic_ostream<wchar_t, char_traits<wchar_t> >&
    __cdecl flush(basic_ostream<wchar_t, char_traits<wchar_t> >& outputStream) {
        outputStream.flush();
        return (outputStream);
    }
}

namespace std {
    template<class element_type, class Traits>
    class basic_istream : virtual public basic_ios<element_type, Traits> {
    public:
        typedef basic_istream<element_type, Traits> MyType;
        typedef basic_ios<element_type, Traits> MyIOSType;
        typedef basic_streambuf<element_type, Traits> MyStreamBufferType;
        typedef istreambuf_iterator<element_type, Traits> IteratorType;
        typedef ctype<element_type> Ctype;
        typedef num_get<element_type, IteratorType> num_get_type;
        explicit  basic_istream(MyStreamBufferType* streamBuffer, bool isStd = false) : charCount_(0) {
            MyIOSType::init(streamBuffer, isStd);
        }

        basic_istream(Uninitialized) {
            ios_base::_Addstd(this);
        }

        virtual  ~basic_istream() {
        }

        typedef typename Traits::int_type int_type;
        typedef typename Traits::pos_type pos_type;
        typedef typename Traits::off_type off_type;
        class Sentry_base {
        public:
            Sentry_base(MyType& inputStream)
                : myInputStream_(inputStream) {
                if (myInputStream_.rdbuf() != 0)
                    myInputStream_.rdbuf()->Lock();
            }

            ~Sentry_base() {
                if (myInputStream_.rdbuf() != 0)
                    myInputStream_.rdbuf()->Unlock();
            }

            MyType& myInputStream_;
        };

        class sentry : public Sentry_base {
        public:
            explicit  sentry(MyType& inputStream, bool noSkip = false)
                : Sentry_base(inputStream) {
                ok = this->myInputStream_._Ipfx(noSkip);
            }

            operator bool() const {
                return (ok);
            }

        private:
            sentry(const sentry&);
            sentry&  operator=(const sentry&);

            bool ok;
        };

        bool  _Ipfx(bool noSkip = false) {
            if (ios_base::good()) {
                if (MyIOSType::tie() != 0)
                    MyIOSType::tie()->flush();

                if (!noSkip && ios_base::flags() & ios_base::skipws) {
                    const Ctype& ctypeFacet = use_facet<Ctype >(ios_base::getloc());

                    try {
                        int_type meta = MyIOSType::rdbuf()->sgetc();

                        for (; ; meta = MyIOSType::rdbuf()->snextc())
                            if (Traits::eq_int_type(Traits::eof(), meta)) {
                                MyIOSType::setstate(ios_base::eofbit);
                                break;
                            } else if (!ctypeFacet.is(Ctype::space,
                                                      Traits::to_char_type(meta)))
                                break;
                    } catch (...) {
                        MyIOSType::setstate(ios_base::badbit, true);
                    }
                }

                if (ios_base::good())
                    return (true);
            }

            MyIOSType::setstate(ios_base::failbit);
            return (false);
        }

        bool  ipfx(bool noSkip = false) {
            return _Ipfx(noSkip);
        }

        void  isfx() {
        }

        MyType&  operator>>(MyType& (__cdecl* Pfn)(MyType&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\istream", 157);
            return ((*Pfn)(*this));
        }

        MyType&  operator>>(MyIOSType& (__cdecl* Pfn)(MyIOSType&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\istream", 163);
            (*Pfn)(*(MyIOSType*)this);
            return (*this);
        }

        MyType&  operator>>(ios_base& (__cdecl* Pfn)(ios_base&)) {
            Debug_pointer(Pfn, L"..\\vc\\include\\istream", 170);
            (*Pfn)(*(ios_base*)this);
            return (*this);
        }

        MyType&  operator>>(_Bool& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(short& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                long temp = 0;
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, temp);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }

                if (state & ios_base::failbit
                        || temp < (-32768) || 32767 < temp)
                    state |= ios_base::failbit;
                else
                    val = (short)temp;
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(unsigned short& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(int& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                long temp = 0;
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, temp);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }

                if (state & ios_base::failbit
                        || temp < (-2147483647 - 1) || 2147483647 < temp)
                    state |= ios_base::failbit;
                else
                    val = temp;
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(unsigned int& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(long& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(unsigned long __w64& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }
        MyType&  operator>>(__int64& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(unsigned __int64& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }
        MyType&  operator>>(float& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(double& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(long double& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(void *& val) {
            ios_base::iostate state = ios_base::goodbit;
            const sentry ok(*this);

            if (ok) {
                const num_get_type& _Nget_fac = use_facet<num_get_type >(ios_base::getloc());

                try {
                    _Nget_fac.get(IteratorType(MyIOSType::rdbuf()), IteratorType(0),
                                  *this, state, val);
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  operator>>(MyStreamBufferType* streamBuffer) {
            ios_base::iostate state = ios_base::goodbit;
            bool copied = false;
            const sentry ok(*this);

            if (ok && streamBuffer != 0) {
                try {
                    int_type meta = MyIOSType::rdbuf()->sgetc();

                    for (; ; meta = MyIOSType::rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else {
                            try {
                                if (Traits::eq_int_type(Traits::eof(),
                                                        streamBuffer->sputc(Traits::to_char_type(meta))))
                                    break;
                            } catch (...) {
                                break;
                            }

                            copied = true;
                        }
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(!copied ? state | ios_base::failbit : state);
            return (*this);
        }

        int_type  get() {
            int_type meta = 0;
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (!ok)
                meta = Traits::eof();
            else {
                try {
                    meta = MyIOSType::rdbuf()->sbumpc();

                    if (Traits::eq_int_type(Traits::eof(), meta))
                        state |= ios_base::eofbit | ios_base::failbit;
                    else
                        ++charCount_;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (meta);
        }

        MyType&  get(element_type* str, streamsize count) {
            return (get(str, count, MyIOSType::widen('\n')));
        }

        MyType&  get(element_type* str, streamsize count, element_type delim) {
            Debug_pointer(str, L"..\\vc\\include\\istream", 509);
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok && 0 < count) {
                try {
                    int_type meta = MyIOSType::rdbuf()->sgetc();

                    for (; 0 < --count; meta = MyIOSType::rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else if (Traits::to_char_type(meta) == delim)
                            break;
                        else {
                            *str++ = Traits::to_char_type(meta);
                            ++charCount_;
                        }
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(charCount_ == 0 ? state | ios_base::failbit : state);
            *str = element_type();
            return (*this);
        }

        MyType&  get(element_type& chr) {
            int_type meta = get();

            if (!Traits::eq_int_type(Traits::eof(), meta))
                chr = Traits::to_char_type(meta);

            return (*this);
        }

        MyType&  get(MyStreamBufferType& streamBuffer) {
            return (get(streamBuffer, MyIOSType::widen('\n')));
        }

        MyType&  get(MyStreamBufferType& streamBuffer, element_type delim) {
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok) {
                try {
                    int_type meta = MyIOSType::rdbuf()->sgetc();

                    for (; ; meta = MyIOSType::rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else {
                            try {
                                element_type chr = Traits::to_char_type(meta);

                                if (chr == delim
                                        || Traits::eq_int_type(Traits::eof(),
                                                               streamBuffer.sputc(chr)))
                                    break;
                            } catch (...) {
                                break;
                            }

                            ++charCount_;
                        }
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            if (charCount_ == 0)
                state |= ios_base::failbit;

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  getline(element_type* str, streamsize count) {
            return (getline(str, count, MyIOSType::widen('\n')));
        }

        MyType&  getline(element_type* str, streamsize count, element_type delim) {
            Debug_pointer(str, L"..\\vc\\include\\istream", 601);
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok && 0 < count) {
                int_type metaDelim = Traits::to_int_type(delim);

                try {
                    int_type meta = MyIOSType::rdbuf()->sgetc();

                    for (; ; meta = MyIOSType::rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else if (meta == metaDelim) {
                            ++charCount_;
                            MyIOSType::rdbuf()->sbumpc();
                            break;
                        } else if (--count <= 0) {
                            state |= ios_base::failbit;
                            break;
                        } else {
                            ++charCount_;
                            *str++ = Traits::to_char_type(meta);
                        }
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            *str = element_type();
            MyIOSType::setstate(charCount_ == 0 ? state | ios_base::failbit : state);
            return (*this);
        }

        MyType&  ignore(streamsize count = 1, int_type metaDelim = Traits::eof()) {
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok && 0 < count) {
                try {
                    for (; ;) {
                        int_type meta;

                        if (count != 2147483647 && --count < 0)
                            break;
                        else if (Traits::eq_int_type(Traits::eof(),
                                                     meta = MyIOSType::rdbuf()->sbumpc())) {
                            state |= ios_base::eofbit;
                            break;
                        } else {
                            ++charCount_;

                            if (meta == metaDelim)
                                break;
                        }
                    }
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  _Read_s(element_type* str, size_t _Str_size, streamsize count) {
            Debug_pointer(str, L"..\\vc\\include\\istream", 680);
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok) {
                try {
                    const streamsize _Num = MyIOSType::rdbuf()->_Sgetn_s(str, _Str_size, count);
                    charCount_ += _Num;

                    if (_Num != count)
                        state |= ios_base::eofbit | ios_base::failbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  read(element_type* str, streamsize count) {
            return _Read_s(str, (size_t) - 1, count);
        }

        streamsize  _Readsome_s(element_type* str, size_t _Str_size, streamsize count) {
            Debug_pointer(str, L"..\\vc\\include\\istream", 706);
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);
            streamsize _Num;

            if (!ok)
                state |= ios_base::failbit;
            else if ((_Num = MyIOSType::rdbuf()->in_avail()) < 0)
                state |= ios_base::eofbit;
            else if (0 < _Num)
                _Read_s(str, _Str_size, _Num < count ? _Num : count);

            MyIOSType::setstate(state);
            return (gcount());
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        streamsize  readsome(element_type* str, streamsize count) {
            return _Readsome_s(str, (size_t) - 1, count);
        }

        int_type  peek() {
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            int_type meta = 0;
            const sentry ok(*this, true);

            if (!ok)
                meta = Traits::eof();
            else {
                try {
                    if (Traits::eq_int_type(Traits::eof(),
                                            meta = MyIOSType::rdbuf()->sgetc()))
                        state |= ios_base::eofbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (meta);
        }

        MyType&  putback(element_type chr) {
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok) {
                try {
                    if (Traits::eq_int_type(Traits::eof(),
                                            MyIOSType::rdbuf()->sputbackc(chr)))
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        MyType&  unget() {
            ios_base::iostate state = ios_base::goodbit;
            charCount_ = 0;
            const sentry ok(*this, true);

            if (ok) {
                try {
                    if (Traits::eq_int_type(Traits::eof(),
                                            MyIOSType::rdbuf()->sungetc()))
                        state |= ios_base::badbit;
                } catch (...) {
                    MyIOSType::setstate(ios_base::badbit, true);
                }
            }

            MyIOSType::setstate(state);
            return (*this);
        }

        streamsize  gcount() const {
            return (charCount_);
        }

        int  sync() {
            ios_base::iostate state = ios_base::goodbit;
            int _Ans;

            if (MyIOSType::rdbuf() == 0)
                _Ans = -1;
            else if (MyIOSType::rdbuf()->pubsync() == -1) {
                state |= ios_base::badbit;
                _Ans = -1;
            } else
                _Ans = 0;

            MyIOSType::setstate(state);
            return (_Ans);
        }

        MyType&  seekg(pos_type position) {
            if (!ios_base::fail()
                    && (off_type)MyIOSType::rdbuf()->pubseekpos(position,
                            ios_base::in) == _BADOFF)
                MyIOSType::setstate(ios_base::failbit);
            else
                MyIOSType::clear();

            return (*this);
        }

        MyType&  seekg(off_type offset, ios_base::seekdir way) {
            if (!ios_base::fail()
                    && (off_type)MyIOSType::rdbuf()->pubseekoff(offset, way,
                            ios_base::in) == _BADOFF)
                MyIOSType::setstate(ios_base::failbit);
            else
                MyIOSType::clear();

            return (*this);
        }

        pos_type  tellg() {
            if (!ios_base::fail())
                return (MyIOSType::rdbuf()->pubseekoff(0,
                                                       ios_base::cur, ios_base::in));
            else
                return (pos_type(_BADOFF));
        }

    private:
        streamsize charCount_;
    };

    template class __declspec(dllimport) basic_istream<char, char_traits<char> >;
    template class __declspec(dllimport) basic_istream<wchar_t, char_traits<wchar_t> >;
    template<class element_type, class Traits>
    class basic_iostream
        : public basic_istream<element_type, Traits>
            , public basic_ostream<element_type, Traits> {
    public:
        typedef element_type char_type;
        typedef Traits traits_type;
        typedef typename Traits::int_type int_type;
        typedef typename Traits::pos_type pos_type;
        typedef typename Traits::off_type off_type;

        explicit  basic_iostream(basic_streambuf<element_type, Traits> *streamBuffer)
            : basic_istream<element_type, Traits>(streamBuffer, false)
            , basic_ostream<element_type, Traits>(NoInit, false) {
        }

        virtual  ~basic_iostream() {
        }
    };

    template class __declspec(dllimport) basic_iostream<char, char_traits<char> >;
    template class __declspec(dllimport) basic_iostream<wchar_t, char_traits<wchar_t> >;
    template<class element_type, class Traits> inline
    basic_istream<element_type, Traits>& __cdecl operator>>(basic_istream<element_type, Traits>& inputStream, element_type* str) {
        Debug_pointer(str, L"..\\vc\\include\\istream", 937);
        typedef basic_istream<element_type, Traits> MyInputStreamType;
        typedef ctype<element_type> Ctype;
        ios_base::iostate state = ios_base::goodbit;
        element_type* str0 = str;
        const typename MyInputStreamType::sentry ok(inputStream);

        if (ok) {
            const Ctype& ctypeFacet = use_facet<Ctype >(inputStream.getloc());

            try {
                streamsize count = 0 < inputStream.width() ? inputStream.width() : 2147483647;
                typename MyInputStreamType::int_type meta = inputStream.rdbuf()->sgetc();
                element_type chr;

                for (; 0 < --count; meta = inputStream.rdbuf()->snextc())
                    if (Traits::eq_int_type(Traits::eof(), meta)) {
                        state |= ios_base::eofbit;
                        break;
                    } else if (ctypeFacet.is(Ctype::space,
                                             chr = Traits::to_char_type(meta))
                               || chr == element_type())
                        break;
                    else
                        *str++ = Traits::to_char_type(meta);
            } catch (...) {
                (inputStream).setstate(ios_base::badbit, true);
            }
        }

        *str = element_type();
        inputStream.width(0);
        inputStream.setstate(str == str0 ? state | ios_base::failbit : state);
        return (inputStream);
    }

    template<class element_type, class Traits> inline
    basic_istream<element_type, Traits>& __cdecl  operator>>(basic_istream<element_type, Traits>& inputStream, element_type& chr) {
        typedef basic_istream<element_type, Traits> MyInputStreamType;
        typename MyInputStreamType::int_type meta;
        ios_base::iostate state = ios_base::goodbit;
        const typename MyInputStreamType::sentry ok(inputStream);

        if (ok) {
            try {
                meta = inputStream.rdbuf()->sbumpc();

                if (Traits::eq_int_type(Traits::eof(), meta))
                    state |= ios_base::eofbit | ios_base::failbit;
                else
                    chr = Traits::to_char_type(meta);
            } catch (...) {
                (inputStream).setstate(ios_base::badbit, true);
            }
        }

        inputStream.setstate(state);
        return (inputStream);
    }
    template<class Traits> inline
    basic_istream<char, Traits>& __cdecl  operator>>(basic_istream<char, Traits>& inputStream, signed char* str) {
        return (inputStream >> (char*)str);
    }

    template<class Traits> inline
    basic_istream<char, Traits>& __cdecl  operator>>(
        basic_istream<char, Traits>& inputStream, signed char& chr) {
        return (inputStream >> (char&)chr);
    }

    template<class Traits> inline
    basic_istream<char, Traits>& __cdecl  operator>>(
        basic_istream<char, Traits>& inputStream, unsigned char* str) {
        return (inputStream >> (char*)str);
    }

    template<class Traits> inline
    basic_istream<char, Traits>& __cdecl  operator>>(
        basic_istream<char, Traits>& inputStream, unsigned char& chr) {
        return (inputStream >> (char&)chr);
    }
    template<class element_type, class Traits> inline
    basic_istream<element_type, Traits>&
    __cdecl ws(basic_istream<element_type, Traits>& inputStream) {
        typedef basic_istream<element_type, Traits> MyInputStreamType;
        typedef ctype<element_type> Ctype;

        if (!inputStream.eof()) {
            ios_base::iostate state = ios_base::goodbit;
            const typename MyInputStreamType::sentry ok(inputStream, true);

            if (ok) {
                const Ctype& ctypeFacet = use_facet<Ctype >(inputStream.getloc());

                try {
                    for (typename Traits::int_type meta = inputStream.rdbuf()->sgetc(); ;
                            meta = inputStream.rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else if (!ctypeFacet.is(Ctype::space,
                                                  Traits::to_char_type(meta)))
                            break;
                } catch (...) {
                    (inputStream).setstate(ios_base::badbit, true);
                }
            }

            inputStream.setstate(state);
        }

        return (inputStream);
    }

    __declspec(dllimport) inline basic_istream<char, char_traits<char> >&
    __cdecl ws(basic_istream<char, char_traits<char> >& inputStream) {
        typedef char element_type;
        typedef char_traits<element_type> Traits;

        if (!inputStream.eof()) {
            ios_base::iostate state = ios_base::goodbit;
            const basic_istream<element_type, Traits>::sentry ok(inputStream, true);

            if (ok) {
                const ctype<element_type>& ctypeFacet =
                    use_facet<ctype<element_type> >(inputStream.getloc());

                try {
                    for (Traits::int_type meta = inputStream.rdbuf()->sgetc(); ;
                            meta = inputStream.rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else if (!ctypeFacet.is(ctype<element_type>::space,
                                                  Traits::to_char_type(meta)))
                            break;
                } catch (...) {
                    (inputStream).setstate(ios_base::badbit, true);
                }
            }

            inputStream.setstate(state);
        }

        return (inputStream);
    }

    __declspec(dllimport) inline basic_istream<wchar_t, char_traits<wchar_t> >&
    __cdecl ws(basic_istream<wchar_t, char_traits<wchar_t> >& inputStream) {
        typedef wchar_t element_type;
        typedef char_traits<element_type> Traits;

        if (!inputStream.eof()) {
            ios_base::iostate state = ios_base::goodbit;
            const basic_istream<element_type, Traits>::sentry ok(inputStream, true);

            if (ok) {
                const ctype<element_type>& ctype_facet =
                    use_facet<ctype<element_type> >(inputStream.getloc());

                try {
                    for (Traits::int_type meta = inputStream.rdbuf()->sgetc(); ;
                            meta = inputStream.rdbuf()->snextc())
                        if (Traits::eq_int_type(Traits::eof(), meta)) {
                            state |= ios_base::eofbit;
                            break;
                        } else if (!ctype_facet.is(ctype<element_type>::space,
                                                   Traits::to_char_type(meta)))
                            break;
                } catch (...) {
                    (inputStream).setstate(ios_base::badbit, true);
                }
            }

            inputStream.setstate(state);
        }

        return (inputStream);
    }
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, char*);
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, char&);
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, signed char*);
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, signed char&);
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, unsigned char*);
    template __declspec(dllimport) basic_istream<char, char_traits<char> >& __cdecl
    operator>>(basic_istream<char, char_traits<char> >&, unsigned char&);
    template __declspec(dllimport) basic_istream<wchar_t, char_traits<wchar_t> >& __cdecl
    operator>>(basic_istream<wchar_t, char_traits<wchar_t> >&, wchar_t*);
    template __declspec(dllimport) basic_istream<wchar_t, char_traits<wchar_t> >& __cdecl
    operator>>(basic_istream<wchar_t, char_traits<wchar_t> >&, wchar_t&);
}

namespace std {
    extern __declspec(dllimport) istream* Ptr_cin;
    extern __declspec(dllimport) ostream* Ptr_cout;
    extern __declspec(dllimport) ostream* Ptr_cerr;
    extern __declspec(dllimport) ostream* Ptr_clog;
    class __declspec(dllimport) Winit {
    public:
        __thiscall Winit();
        __thiscall ~Winit();
    private:
        static int initCount_;
    };
    extern __declspec(dllimport) wistream* Ptr_wcin;
    extern __declspec(dllimport) wostream* Ptr_wcout;
    extern __declspec(dllimport) wostream* Ptr_wcerr;
    extern __declspec(dllimport) wostream* Ptr_wclog;

    extern __declspec(dllimport) istream cin;
    extern __declspec(dllimport) ostream cout;
    extern __declspec(dllimport) ostream cerr;
    extern __declspec(dllimport) ostream clog;

    extern __declspec(dllimport) wistream wcin;
    extern __declspec(dllimport) wostream wcout;
    extern __declspec(dllimport) wostream wcerr;
    extern __declspec(dllimport) wostream wclog;

}
