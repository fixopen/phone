namespace std {
    typedef bool _Bool;
#pragma warning(push)
#pragma warning(disable:4412)
    class Lockit {
    public:
        explicit __thiscall Lockit();
        explicit __thiscall Lockit(int);
        __thiscall ~Lockit();
        static  void __cdecl lockit_ctor(int);
        static  void __cdecl lockit_dtor(int);
    private:
        static  void __cdecl lockit_ctor(Lockit*);
        static  void __cdecl lockit_ctor(Lockit*, int);
        static  void __cdecl lockit_dtor(Lockit*);
        Lockit(const Lockit&);
        Lockit& operator=(const Lockit&);
        int locktype_;
    };

    class  Mutex {
    public:
        __thiscall Mutex();
        __thiscall ~Mutex();
        void __thiscall lock();
        void __thiscall unlock();
    private:
        static  void __cdecl mutex_ctor(Mutex*);
        static  void __cdecl mutex_dtor(Mutex*);
        static  void __cdecl mutex_lock(Mutex*);
        static  void __cdecl mutex_unlock(Mutex*);
        Mutex(const Mutex&);
        Mutex&  operator=(const Mutex&);
        void* mutex_;
    };

    class  Init_locks {
    public:
        __thiscall Init_locks();
        __thiscall ~Init_locks();
    private:
        static  void __cdecl init_locks_ctor(Init_locks*);
        static  void __cdecl init_locks_dtor(Init_locks*);
    };

    using ::FILE;
    using ::size_t;
    using ::fpos_t;
    using ::FILE;
    using ::clearerr;
    using ::fclose;
    using ::feof;
    using ::ferror;
    using ::fflush;
    using ::fgetc;
    using ::fgetpos;
    using ::fgets;
    using ::fopen;
    using ::fprintf;
    using ::fputc;
    using ::fputs;
    using ::fread;
    using ::fscanf;
    using ::fseek;
    using ::fsetpos;
    using ::ftell;
    using ::fwrite;
    using ::getchar;
    using ::gets;
    using ::putchar;
    using ::printf;
    using ::puts;
    using ::scanf;
    using ::setvbuf;
    using ::sprintf;
    using ::sscanf;
    using ::ungetc;
    using ::vfprintf;
    using ::vprintf;
    using ::vsprintf;

    using ::size_t;
    using ::memchr;
    using ::memcmp;
    using ::memcpy;
    using ::memmove;
    using ::memset;
    using ::strcat;
    using ::strchr;
    using ::strcmp;
    using ::strcpy;
    using ::strcspn;
    using ::strlen;
    using ::strncat;
    using ::strncmp;
    using ::strncpy;
    using ::strpbrk;
    using ::strrchr;
    using ::strspn;
    using ::strstr;
    using ::strtok;

    using ::_Mbstatet;
    using ::mbstate_t;
    using ::size_t;
    using ::tm;
    using ::wint_t;
    using ::fgetwc;
    using ::fgetws;
    using ::fputwc;
    using ::fputws;
    using ::fwide;
    using ::fwprintf;
    using ::fwscanf;
    using ::getwc;
    using ::getwchar;
    using ::mbsinit;
    using ::putwc;
    using ::putwchar;
    using ::swprintf;
    using ::swscanf;
    using ::ungetwc;
    using ::vfwprintf;
    using ::vswprintf;
    using ::vwprintf;
    using ::wprintf;
    using ::wscanf;
    using ::wcstol;
    using ::wcscat;
    using ::wcschr;
    using ::wcscmp;
    using ::wcscpy;
    using ::wcscspn;
    using ::wcslen;
    using ::wcsncat;
    using ::wcsncmp;
    using ::wcsncpy;
    using ::wcspbrk;
    using ::wcsrchr;
    using ::wcsspn;
    using ::wcstod;
    using ::wcstoul;
    using ::wcsstr;
    using ::wcstok;
    using ::wmemchr;
    using ::wmemcmp;
    using ::wmemcpy;
    using ::wmemmove;
    using ::wmemset;
    using ::wcsftime;

    using ::ptrdiff_t;
    using ::size_t;

    enum uninitialized {
        noinit
    };

    typedef long streamoff;
    typedef int streamsize;

    extern fpos_t file_position;
    extern const streamoff bad_off;

    template<class TState>
    class fpos {
        typedef fpos<TState> MyType;
    public:
        fpos(streamoff offset = 0) : offset_(offset), pos_(0), state_(staticState_) {
        }
        fpos(TState state, fpos_t filePosition) : offset_(0), pos_(filePosition), state_(state) {
        }
        TState state() const {
            return (state_);
        }
        void  state(TState state) {
            state_ = state;
        }
        fpos_t seekpos() const {
            return (pos_);
        }
        operator streamoff() const {
            return ((streamoff)(offset_ + ((long)(pos_))));
        }
        streamoff  operator-(const MyType& right) const {
            return ((streamoff) * this - (streamoff)right);
        }
        MyType&  operator+=(streamoff offset) {
            offset_ += offset;
            return (*this);
        }
        MyType&  operator-=(streamoff offset) {
            offset_ -= offset;
            return (*this);
        }
        MyType operator+(streamoff offset) const {
            MyType t = *this;
            return (t += offset);
        }
        MyType operator-(streamoff offset) const {
            MyType t = *this;
            return (t -= offset);
        }
        bool  operator==(const MyType& right) const {
            return ((streamoff) * this == (streamoff)right);
        }
        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }
    private:
        static const TState staticState_;
        streamoff offset_;
        fpos_t pos_;
        TState state_;
    };

    template<class TState>
    const TState fpos<TState>::staticState_ = TState();
    typedef fpos<mbstate_t> streampos;
    typedef streampos wstreampos;

    struct unsecure_char_traits_tag {
    };

    struct secure_char_traits_tag {
    };

    struct char_traits_base {
        typedef unsecure_char_traits_tag secure_char_traits;
    };

    template<class TElement>
    struct char_traits : public char_traits_base {
        typedef TElement char_type;
        typedef long int_type;
        typedef streampos pos_type;
        typedef streamoff off_type;
        typedef mbstate_t state_type;
        static void __cdecl assign(TElement& left, const TElement& right) {
            left = right;
        }

        static bool __cdecl eq(const TElement& left, const TElement& right) {
            return (left == right);
        }

        static bool __cdecl lt(const TElement& left, const TElement& right) {
            return (left < right);
        }

        static int __cdecl compare(const TElement* first1, const TElement* first2, size_t count) {
            for (; 0 < count; --count, ++first1, ++first2)
                if (!eq(*first1, *first2))
                    return (lt(*first1, *first2) ? -1 : +1);

            return (0);
        }

        static size_t __cdecl length(const TElement* first) {
            size_t count;

            for (count = 0; !eq(*first, TElement()); ++first)
                ++count;

            return (count);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl copy(TElement* first1, const TElement* first2, size_t count) {
            return copy_s(first1, count, first2, count);
        }

        static TElement* __cdecl copy_s(TElement* first1, size_t destinationSize, const TElement* first2, size_t count) {
            {
                if (!(destinationSize >= count)) {
                    (void)(((("destinationSize >= count", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iosfwd", 221, 0, L"(\"destinationSize >= count\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"destinationSize >= count", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iosfwd", 221, 0);
                    return (0);
                }
            }
            TElement* next = first1;

            for (; 0 < count; --count, ++next, ++first2)
                assign(*next, *first2);

            return (first1);
        }

        static const TElement* __cdecl find(const TElement* first, size_t count, const TElement& chr) {
            for (; 0 < count; --count, ++first)
                if (eq(*first, chr))
                    return (first);

            return (0);
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl move(TElement* first1, const TElement* first2, size_t count) {
            return move_s(first1, count, first2, count);
        }

        static TElement* __cdecl move_s(TElement* first1, size_t destinationSize, const TElement* first2, size_t count) {
            {
                if (!(destinationSize >= count)) {
                    (void)(((("destinationSize >= count", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iosfwd", 251, 0, L"(\"destinationSize >= count\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"destinationSize >= count", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iosfwd", 251, 0);
                    return (0);
                }
            }
            TElement* next = first1;

            if (first2 < next && next < first2 + count)
                for (next += count, first2 += count; 0 < count; --count)
                    assign(*--next, *--first2);
            else
                for (; 0 < count; --count, ++next, ++first2)
                    assign(*next, *first2);

            return (first1);
        }

        static TElement* __cdecl assign(TElement* first, size_t count, TElement chr) {
            TElement* next = first;

            for (; 0 < count; --count, ++next)
                assign(*next, chr);

            return (first);
        }

        static TElement __cdecl to_char_type(const int_type& meta) {
            return ((TElement)meta);
        }

        static int_type __cdecl to_int_type(const TElement& chr) {
            return ((int_type)chr);
        }

        static bool __cdecl eq_int_type(const int_type& left, const int_type& right) {
            return (left == right);
        }

        static int_type __cdecl eof() {
            return ((int_type)(-1));
        }

        static int_type __cdecl not_eof(const int_type& meta) {
            return (meta != eof() ? (int_type)meta : (int_type)!eof());
        }
    };

    template<> struct char_traits<wchar_t> : public char_traits_base {
        typedef wchar_t TElement;
        typedef TElement char_type;
        typedef wint_t int_type;
        typedef streampos pos_type;
        typedef streamoff off_type;
        typedef mbstate_t state_type;
        static void __cdecl assign(TElement& left, const TElement& right) {
            left = right;
        }

        static bool __cdecl eq(const TElement& left, const TElement& right) {
            return (left == right);
        }

        static bool __cdecl lt(const TElement& left, const TElement& right) {
            return (left < right);
        }

        static int __cdecl compare(const TElement* first1, const TElement* first2, size_t count) {
            return (::wmemcmp(first1, first2, count));
        }

        static size_t __cdecl length(const TElement* first) {
            return (::wcslen(first));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl copy(TElement* first1, const TElement* first2, size_t count) {
            return copy_s(first1, count, first2, count);
        }

        static TElement* __cdecl copy_s(TElement* first1, size_t sizeInWords, const TElement* first2, size_t count) {
            ::wmemcpy_s((first1), (sizeInWords), (first2), (count));
            return first1;
        }

        static const TElement* __cdecl find(const TElement* first, size_t count, const TElement& chr) {
            return ((const TElement*)::wmemchr(first, chr, count));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl move(TElement* first1, const TElement* first2, size_t count) {
            return move_s(first1, count, first2, count);
        }

        static TElement* __cdecl move_s(TElement* first1, size_t sizeInWords, const TElement* first2, size_t count) {
            ::wmemmove_s((first1), (sizeInWords), (first2), (count));
            return (TElement*)first1;
        }

        static TElement* __cdecl assign(TElement* first, size_t count, TElement chr) {
            return ((TElement*)::wmemset(first, chr, count));
        }

        static TElement __cdecl to_char_type(const int_type& meta) {
            return (meta);
        }

        static int_type __cdecl to_int_type(const TElement& chr) {
            return (chr);
        }

        static bool __cdecl eq_int_type(const int_type& left, const int_type& right) {
            return (left == right);
        }

        static int_type __cdecl eof() {
            return ((wint_t)(0xFFFF));
        }

        static int_type __cdecl not_eof(const int_type& meta) {
            return (meta != eof() ? meta : !eof());
        }
    };

    template<> struct char_traits<char> : public char_traits_base {
        typedef char TElement;
        typedef TElement char_type;
        typedef int int_type;
        typedef streampos pos_type;
        typedef streamoff off_type;
        typedef _Mbstatet state_type;
        static void __cdecl assign(TElement& left, const TElement& right) {
            left = right;
        }

        static bool __cdecl eq(const TElement& left, const TElement& right) {
            return (left == right);
        }

        static bool __cdecl lt(const TElement& left, const TElement& right) {
            return (left < right);
        }

        static int __cdecl compare(const TElement* first1, const TElement* first2, size_t count) {
            return (::memcmp(first1, first2, count));
        }

        static size_t __cdecl length(const TElement* first) {
            return (::strlen(first));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl copy(TElement* first1, const TElement* first2, size_t count) {
            return copy_s(first1, count, first2, count);
        }

        static TElement* __cdecl copy_s(TElement* first1, size_t sizeInBytes, const TElement* first2, size_t count) {
            ::memcpy_s((first1), (sizeInBytes), (first2), (count));
            return first1;
        }

        static const TElement* __cdecl find(const TElement* first, size_t count, const TElement& chr) {
            return ((const TElement*)::memchr(first, chr, count));
        }

        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        static TElement* __cdecl move(TElement* first1, const TElement* first2, size_t count) {
            return move_s(first1, count, first2, count);
        }

        static TElement* __cdecl move_s(TElement* first1, size_t sizeInBytes, const TElement* first2, size_t count) {
            ::memmove_s((first1), (sizeInBytes), (first2), (count));
            return first1;
        }

        static TElement* __cdecl assign(TElement* first, size_t count, TElement chr) {
            return ((TElement*)::memset(first, chr, count));
        }

        static TElement __cdecl to_char_type(const int_type& meta) {
            return ((TElement)meta);
        }

        static int_type __cdecl to_int_type(const TElement& chr) {
            return ((unsigned char)chr);
        }

        static bool __cdecl eq_int_type(const int_type& left, const int_type& right) {
            return (left == right);
        }

        static int_type __cdecl eof() {
            return ((-1));
        }

        static int_type __cdecl not_eof(const int_type& meta) {
            return (meta != eof() ? meta : !eof());
        }
    };

    template <class TTraits>
    class inherits_from_char_traits_base {
        typedef char True;
        class False {
            char Dummy[2];
        };
        static True inherits(char_traits_base);
        static False inherits(...);
        static TTraits make_traits();
    public:
        enum { Exists = sizeof(inherits(make_traits())) == sizeof(True) };
    };

    template <class TTraits, bool inherits_from_char_traits_base>
    class char_traits_category_helper {
    public:
        typedef unsecure_char_traits_tag secure_char_traits;
    };

    template <class TElement>
    class char_traits_category_helper<char_traits<TElement>, true> {
    public:
        typedef secure_char_traits_tag secure_char_traits;
    };

    template <class TTraits>
    class char_traits_category_helper<TTraits, true> {
    public:
        typedef typename TTraits::secure_char_traits secure_char_traits;
    };

    template <class TTraits>
    class char_traits_category {
    public:
        typedef typename char_traits_category_helper<TTraits, inherits_from_char_traits_base<TTraits>::Exists>::secure_char_traits secure_char_traits;
    };

    template<class TTraits>
    inline typename char_traits_category<TTraits>::secure_char_traits char_traits_category() {
        typename char_traits_category<TTraits>::secure_char_traits secure;
        return (secure);
    }

    namespace traits_helper {
        template<class TTraits>
        inline typename TTraits::char_type* copy_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count) {
            return copy_s<TTraits>(first1, size, first2, count, char_traits_category<TTraits>());
        }

        template<class TTraits>
        inline typename TTraits::char_type* copy_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count, secure_char_traits_tag) {
            return TTraits::copy_s(first1, size, first2, count);
        }

        template<class TTraits>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename TTraits::char_type* copy_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count, unsecure_char_traits_tag) {
            return TTraits::copy(first1, first2, count);
        }

        template<class TTraits>
        inline typename TTraits::char_type* move_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count) {
            return move_s<TTraits>(first1, size, first2, count, char_traits_category<TTraits>());
        }

        template<class TTraits>
        inline typename TTraits::char_type* move_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count, secure_char_traits_tag) {
            return TTraits::move_s(first1, size, first2, count);
        }

        template<class TTraits>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename TTraits::char_type* move_s(typename TTraits::char_type* first1, size_t size, const typename TTraits::char_type* first2, size_t count, unsecure_char_traits_tag) {
            return TTraits::move(first1, first2, count);
        }
    }
    template<class T>
    class allocator;
    class ios_base;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_ios;
    template < class TElement, class TTraits = char_traits<TElement> >
    class istreambuf_iterator;
    template < class TElement, class TTraits = char_traits<TElement> >
    class ostreambuf_iterator;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_streambuf;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_istream;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_ostream;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_iostream;
    template < class TElement, class TTraits = char_traits<TElement>, class TAlloc = allocator<TElement> >
    class basic_stringbuf;
    template < class TElement, class TTraits = char_traits<TElement>, class TAlloc = allocator<TElement> >
    class basic_istringstream;
    template < class TElement, class TTraits = char_traits<TElement>, class TAlloc = allocator<TElement> >
    class basic_ostringstream;
    template < class TElement, class TTraits = char_traits<TElement>, class TAlloc = allocator<TElement> >
    class basic_stringstream;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_filebuf;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_ifstream;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_ofstream;
    template < class TElement, class TTraits = char_traits<TElement> >
    class basic_fstream;

    typedef basic_ios<char, char_traits<char> > ios;
    typedef basic_streambuf<char, char_traits<char> > streambuf;
    typedef basic_istream<char, char_traits<char> > istream;
    typedef basic_ostream<char, char_traits<char> > ostream;
    typedef basic_iostream<char, char_traits<char> > iostream;
    typedef basic_stringbuf<char, char_traits<char>, allocator<char> > stringbuf;
    typedef basic_istringstream<char, char_traits<char>, allocator<char> > istringstream;
    typedef basic_ostringstream<char, char_traits<char>, allocator<char> > ostringstream;
    typedef basic_stringstream<char, char_traits<char>, allocator<char> > stringstream;
    typedef basic_filebuf<char, char_traits<char> > filebuf;
    typedef basic_ifstream<char, char_traits<char> > ifstream;
    typedef basic_ofstream<char, char_traits<char> > ofstream;
    typedef basic_fstream<char, char_traits<char> > fstream;
    typedef basic_ios<wchar_t, char_traits<wchar_t> > wios;
    typedef basic_streambuf<wchar_t, char_traits<wchar_t> > wstreambuf;
    typedef basic_istream<wchar_t, char_traits<wchar_t> > wistream;
    typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;
    typedef basic_iostream<wchar_t, char_traits<wchar_t> > wiostream;
    typedef basic_stringbuf<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstringbuf;
    typedef basic_istringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wistringstream;
    typedef basic_ostringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wostringstream;
    typedef basic_stringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstringstream;
    typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;
    typedef basic_ifstream<wchar_t, char_traits<wchar_t> > wifstream;
    typedef basic_ofstream<wchar_t, char_traits<wchar_t> > wofstream;
    typedef basic_fstream<wchar_t, char_traits<wchar_t> > wfstream;

    template<class T> inline void swap(T& left, T& right) {
        if (&left != &right) {
            T temp = left;
            left = right;
            right = temp;
        }
    }

    template<class T1, class T2> struct pair {
        typedef pair<T1, T2> MyType;
        typedef T1 first_type;
        typedef T2 second_type;
        pair() : first(T1()), second(T2()) {
        }

        pair(const T1& value1, const T2& value2) : first(value1), second(value2) {
        }

        template<class OT1, class OT2>
        pair(const pair<OT1, OT1>& right) : first(right.first), second(right.second) {
        }

        void swap(MyType& right) {
            if (this != &right) {
                std::swap(first, right.first);
                std::swap(second, right.second);
            }
        }
        T1 first;
        T2 second;
    };

    template<class T1, class T2>
    inline bool operator==(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (left.first == right.first && left.second == right.second);
    }

    template<class T1, class T2>
    inline bool operator!=(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (!(left == right));
    }

    template<class T1, class T2>
    inline bool operator<(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (left.first < right.first || !(right.first < left.first) && left.second < right.second);
    }
    template<class T1, class T2>
    inline bool operator>(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (right < left);
    }

    template<class T1, class T2>
    inline bool operator<=(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (!(right < left));
    }

    template<class T1, class T2>
    inline bool operator>=(const pair<T1, T2>& left, const pair<T1, T2>& right) {
        return (!(left < right));
    }

    template<class T1, class T2>
    inline pair<T1, T2> make_pair(T1 value1, T2 value2) {
        return (pair<T1, T2>(value1, value2));
    }

    template<class T1, class T2>
    inline void swap(pair<T1, T2>& left, pair<T1, T2>& right) {
        left.swap(right);
    }

    namespace rel_ops {
        template<class T>
        inline bool operator!=(const T& left, const T& right) {
            return (!(left == right));
        }

        template<class T>
        inline bool operator>(const T& left, const T& right) {
            return (right < left);
        }

        template<class T>
        inline bool operator<=(const T& left, const T& right) {
            return (!(right < left));
        }

        template<class T>
        inline bool operator>=(const T& left, const T& right) {
            return (!(left < right));
        }
    }

    struct unchecked_iterator_tag {
    };

    struct range_checked_iterator_tag {
    };

    struct unchanged_checked_iterator_base_type_tag {
    };

    struct different_checked_iterator_base_type_tag {
    };

    struct undefined_inner_type_tag {
    };

    struct undefinedmove_tag {
    };

    struct swapmove_tag {
    };

    class iterator_base_universal {
    public:
        typedef unchecked_iterator_tag checked_iterator_category;
        typedef unchanged_checked_iterator_base_type_tag checked_iterator_base_type;
        typedef undefined_inner_type_tag inner_type;
    };

    __declspec(noreturn) __declspec(deprecated) void __cdecl debug_message(const char*, const char*);
    void __cdecl debug_message(const wchar_t*, const wchar_t*, unsigned int line);
    class iterator_base;
    class container_base_secure {
    public:
        friend class iterator_base;
        container_base_secure() : myFirstIterator_(0) {
        }

        container_base_secure(const container_base_secure&) : myFirstIterator_(0) {
        }

        container_base_secure&  operator=(const container_base_secure&) {
            return (*this);
        }

        ~container_base_secure() {
            orphan_all();
        }

        void  orphan_all() const;
        void  swap_all(container_base_secure&) const;
        void  swap_aux(container_base_secure&) {
        }

        iterator_base* myFirstIterator_;
    };

    typedef container_base_secure container_base;
    class iterator_base : public iterator_base_universal {
    public:
        friend class container_base_secure;
        iterator_base() : myContainer_(0), myNextIterator_(0) {
        }

        iterator_base(const iterator_base& right) : myContainer_(0), myNextIterator_(0) {
            *this = right;
        }

        iterator_base&  operator=(const iterator_base& right) {
            if (myContainer_ != right.myContainer_) {
                lockit lock(3);
                orphan_me();
                adopt(right.myContainer_);
            }

            return (*this);
        }

        ~iterator_base() {
            lockit lock(3);
            orphan_me();
        }

        void  adopt(const container_base_secure* parent) {
            if (myContainer_ != parent) {
                lockit lock(3);
                orphan_me();

                if (parent != 0 && parent->myFirstIterator_ != ((iterator_base*) - 3)) {
                    myNextIter_ = parent->myFirstIterator_;
                    ((container_base_secure*)parent)->myFirstIterator_ = this;
                }

                myContainer_ = parent;
            }
        }

        void  orphan_me() {
            if (myContainer_ != 0 && myContainer_->myFirstIterator_ != ((iterator_base*) - 3)) {
                iterator_base** next = (iterator_base**)&myContainer_->myFirstIterator_;

                while (*next != 0 && *next != this)
                    next = &(*next)->myNextIterator_;

                if (*next == 0)
                    debug_message(L"ITERATOR LIST CORRUPTED!", L"..\\VC\\ce\\include\\xutility", 191);

                *next = myNextIterator_;
                myContainer_ = 0;
            }
        }

        const container_base_secure*   getmycont() const {
            return myContainer_;
        }

        bool  same_container(const iterator_base& other) const {
            return myContainer_ == other.myContainer_;
        }

        bool  has_container() const {
            return myContainer_ != 0;
        }

        const container_base_secure* myContainer_;
        iterator_base* myNextIterator_;
    };

    typedef iterator_base iterator_base_secure;
    inline void container_base_secure::orphan_all() const {
        lockit lock(3);

        if (myFirstIterator_ != ((iterator_base*) - 3)) {
            for (iterator_base** next = (iterator_base**)&myFirstIterator_;
                    *next != 0; *next = (*next)->myNextIterator_)
                (*next)->myContainer_ = 0;

            *(iterator_base**)&myFirstIterator_ = 0;
        }
    }

    inline void container_base_secure::swap_all(container_base_secure& right) const {
        lockit lock(3);
        iterator_base* next;
        iterator_base* temp = (iterator_base*)myFirstIterator_;
        *(iterator_base**)&myFirstIter_ = (iterator_base*)right.myFirstIterator_;
        *(iterator_base**)&right.myFirstIterator_ = temp;

        if (myFirstIterator_ != ((iterator_base*) - 3)) {
            for (next = (iterator_base*)myFirstIterator_; next != 0; next = next->myNextIterator_)
                next->myContainer_ = this;
        }

        if (right.myFirstIter_ != ((iterator_base*) - 3)) {
            for (next = (iterator_base*)right.myFirstIterator_; next != 0; next = next->myNextIterator_)
                next->myContainer_ = &right;
        }
    }

    template<class T1, class T2>
    inline bool __cdecl debug_lt(const T1& left, const T2& right, const wchar_t* where, unsigned int line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class T1, class T2>
    inline bool __cdecl debug_lt(const T1& left, T2& right, const wchar_t* where, unsigned int line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class T1, class T2>
    inline bool __cdecl debug_lt(T1& left, const T2& right, const wchar_t* where, unsigned int line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class T1, class T2>
    inline bool __cdecl debug_lt(T1& left, T2& right, const wchar_t* where, unsigned int line) {
        if (!(left < right))
            return (false);
        else if (right < left)
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class TPred, class T1, class T2>
    inline bool __cdecl debug_lt_pred(TPred pred, const T1& left, const T2& right, const wchar_t* where, unsigned int line) {
        if (!pred(left, right))
            return (false);
        else if (pred(right, left))
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class TPred, class T1, class T2>
    inline bool __cdecl debug_lt_pred(TPred pred, const T1& left, T2& right, const wchar_t* where, unsigned int line) {
        if (!pred(left, right))
            return (false);
        else if (pred(right, left))
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class TPred, class T1, class T2>
    inline bool __cdecl debug_lt_pred(TPred pred, T1& left, const T2& right, const wchar_t* where, unsigned int line) {
        if (!pred(left, right))
            return (false);
        else if (pred(right, left))
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class TPred, class T1, class T2>
    inline bool __cdecl debug_lt_pred(TPred pred, T1& left, T2& right, const wchar_t* where, unsigned int line) {
        if (!pred(left, right))
            return (false);
        else if (pred(right, left))
            debug_message(L"invalid operator<", where, line);

        return (true);
    }

    template<class TAlloc>
    class container_base_aux_alloc_empty : public container_base {
    protected:
        explicit container_base_aux_alloc_empty(TAlloc) { }
        container_base_aux_alloc_empty(const container_base_aux_alloc_empty&) { }
        container_base_aux_alloc_empty& operator=(const container_base_aux_alloc_empty&) {
            return *this;
        }
        ~container_base_aux_alloc_empty() { }
    };

    struct input_iterator_tag {
    };

    struct output_iterator_tag {
    };

    struct forward_iterator_tag : public input_iterator_tag {
    };

    struct bidirectional_iterator_tag : public forward_iterator_tag {
    };

    struct random_access_iterator_tag : public bidirectional_iterator_tag {
    };

    struct int_iterator_tag {
    };

    struct float_iterator_tag {
    };

    struct nonscalar_ptr_iterator_tag {
    };

    struct ncalar_ptr_iterator_tag {
    };

    template < class TCategory, class T, class TDiff = ptrdiff_t, class TPointer = T*, class TReference = T& >
    struct iterator : public iterator_base_universal {
        typedef TCategory iterator_category;
        typedef T value_type;
        typedef TDiff difference_type;
        typedef TDiff distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    template < class TCategory, class T, class TDiff = ptrdiff_t, class TPointer = T*, class TReference = T&, class TBaseClass = iterator_base >
    struct iterator_with_base : public TBaseClass {
        typedef TCategory iterator_category;
        typedef T value_type;
        typedef TDiff difference_type;
        typedef TDiff distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    template<class T, class TDiff, class TPointer, class TReference>
    struct BidirectIterator : public iterator_with_base<bidirectional_iterator_tag, T, TDiff, TPointer, TReference> {
    };

    template<class T, class TDiff, class TPointer, class TReference>
    struct RandomIterator : public iterator_with_base<random_access_iterator_tag, T, TDiff, TPointer, TReference> {
    };

    template<class T, class TDiff, class TPointer, class TReference, class TBaseClass>
    struct RandomIterator_base : public TBaseClass {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef TDiff difference_type;
        typedef TDiff distance_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };

    struct OutputIterator : public iterator<output_iterator_tag, void, void, void, void> {
    };

    template<class TIterator>
    struct iterator_traits {
        typedef typename TIterator::iterator_category iterator_category;
        typedef typename TIterator::value_type value_type;
        typedef typename TIterator::difference_type difference_type;
        typedef difference_type distance_type;
        typedef typename TIterator::pointer pointer;
        typedef typename TIterator::reference reference;
    };

    template<class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
        typedef T* pointer;
        typedef T& reference;
    };

    template<class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef ptrdiff_t distance_type;
        typedef const T* pointer;
        typedef const T& reference;
    };

    template<> struct iterator_traits<bool> {
        typedef int_iterator_tag iterator_category;
        typedef bool value_type;
        typedef bool difference_type;
        typedef bool distance_type;
        typedef bool* pointer;
        typedef bool& reference;
    };

    template<> struct iterator_traits<char> {
        typedef int_iterator_tag iterator_category;
        typedef char value_type;
        typedef char difference_type;
        typedef char distance_type;
        typedef char* pointer;
        typedef char& reference;
    };

    template<> struct iterator_traits<signed char> {
        typedef int_iterator_tag iterator_category;
        typedef signed char value_type;
        typedef signed char difference_type;
        typedef signed char distance_type;
        typedef signed char* pointer;
        typedef signed char& reference;
    };

    template<> struct iterator_traits<unsigned char> {
        typedef int_iterator_tag iterator_category;
        typedef unsigned char value_type;
        typedef unsigned char difference_type;
        typedef unsigned char distance_type;
        typedef unsigned char* pointer;
        typedef unsigned char& reference;
    };

    template<> struct iterator_traits<wchar_t> {
        typedef int_iterator_tag iterator_category;
        typedef wchar_t value_type;
        typedef wchar_t difference_type;
        typedef wchar_t distance_type;
        typedef wchar_t* pointer;
        typedef wchar_t& reference;
    };

    template<> struct iterator_traits<short> {
        typedef int_iterator_tag iterator_category;
        typedef short value_type;
        typedef short difference_type;
        typedef short distance_type;
        typedef short* pointer;
        typedef short& reference;
    };

    template<> struct iterator_traits<unsigned short> {
        typedef int_iterator_tag iterator_category;
        typedef unsigned short value_type;
        typedef unsigned short difference_type;
        typedef unsigned short distance_type;
        typedef unsigned short* pointer;
        typedef unsigned short& reference;
    };

    template<> struct iterator_traits<int> {
        typedef int_iterator_tag iterator_category;
        typedef int value_type;
        typedef int difference_type;
        typedef int distance_type;
        typedef int* pointer;
        typedef int& reference;
    };

    template<> struct iterator_traits<unsigned int> {
        typedef int_iterator_tag iterator_category;
        typedef unsigned int value_type;
        typedef unsigned int difference_type;
        typedef unsigned int distance_type;
        typedef unsigned int* pointer;
        typedef unsigned int& reference;
    };

    template<> struct iterator_traits<long> {
        typedef int_iterator_tag iterator_category;
        typedef long value_type;
        typedef long difference_type;
        typedef long distance_type;
        typedef long* pointer;
        typedef long& reference;
    };

    template<> struct iterator_traits<unsigned long> {
        typedef int_iterator_tag iterator_category;
        typedef unsigned long value_type;
        typedef unsigned long difference_type;
        typedef unsigned long distance_type;
        typedef unsigned long* pointer;
        typedef unsigned long& reference;
    };

    template<> struct iterator_traits<double> {
        typedef float_iterator_tag iterator_category;
        typedef double value_type;
        typedef double difference_type;
        typedef double distance_type;
        typedef double* pointer;
        typedef double& reference;
    };

    template<> struct iterator_traits<float> {
        typedef float_iterator_tag iterator_category;
        typedef float value_type;
        typedef float difference_type;
        typedef float distance_type;
        typedef float* pointer;
        typedef float& reference;
    };

    template<> struct iterator_traits<__int64> {
        typedef int_iterator_tag iterator_category;
        typedef __int64 value_type;
        typedef __int64 difference_type;
        typedef __int64 distance_type;
        typedef __int64* pointer;
        typedef __int64& reference;
    };

    template<> struct iterator_traits<unsigned __int64> {
        typedef int_iterator_tag iterator_category;
        typedef unsigned __int64 value_type;
        typedef unsigned __int64 difference_type;
        typedef unsigned __int64 distance_type;
        typedef unsigned __int64* pointer;
        typedef unsigned __int64& reference;
    };

    template<class TIterator>
    inline typename iterator_traits<TIterator>::iterator_category
    __cdecl iterator_category(const TIterator&) {
        typename iterator_traits<TIterator>::iterator_category category;
        return category;
    }

    template<class TCategory1, class TCategory2>
    class iterator_random_helper {
    public:
        typedef forward_iterator_tag iterator_random_category;
    };

    template<>
    class iterator_random_helper<random_access_iterator_tag, random_access_iterator_tag> {
    public:
        typedef random_access_iterator_tag iterator_random_category;
    };

    template<class TCategory1, class TCategory2, class TCategory3>
    class iterator_random_helper3 {
    public:
        typedef forward_iterator_tag iterator_random_category;
    };

    template<>
    class iterator_random_helper3<random_access_iterator_tag, random_access_iterator_tag, random_access_iterator_tag> {
    public:
        typedef random_access_iterator_tag iterator_random_category;
    };

    template<class TIterator1, class TIterator2>
    inline typename iterator_random_helper<typename iterator_traits<TIterator1>::iterator_category, typename iterator_traits<TIterator2>::iterator_category>::iterator_random_category
    __cdecl iterator_random(const TIterator1&, const TIterator2&) {
        typename iterator_random_helper <
        iterator_traits<TIterator1>::iterator_category,
                        iterator_traits<TIterator2>::iterator_category >::iterator_random_category category;
        return category;
    }

    template<class TIterator1, class TIterator2, class TIterator3>
    inline typename iterator_random_helper3 <
    typename iterator_traits<TIterator1>::iterator_category,
             typename iterator_traits<TIterator2>::iterator_category,
             typename iterator_traits<TIterator3>::iterator_category >::iterator_random_category
    __cdecl iterator_random(const TIterator1&, const TIterator2&, const TIterator3&) {
        typename iterator_random_helper3 <
        iterator_traits<TIterator1>::iterator_category,
                        iterator_traits<TIterator2>::iterator_category,
                        iterator_traits<TIterator3>::iterator_category >::iterator_random_category category;
        return category;
    }

    template <bool TCondition, class T1, class T2>
    class If {
    public:
        typedef T2 result;
    };

    template <class T1, class T2>
    class If<true, T1, T2> {
    public:
        typedef T1 result;
    };

    template <bool secure_validation>
    class secure_validation_helper {
    public:
        typedef unchecked_iterator_tag checked_iterator_category;
    };

    template <>
    class secure_validation_helper<true> {
    public:
        typedef range_checked_iterator_tag checked_iterator_category;
    };

    template <class TIterator, bool inherits_from_iterator_base>
    class checked_iterator_category_helper {
    public:
        typedef unchecked_iterator_tag checked_category;
    };

    template <class TIterator>
    class checked_iterator_category_helper<TIterator, true> {
    public:
        typedef typename TIterator::checked_iterator_category checked_category;
    };

    template <class TIterator>
    class checked_iterator_category {
    public:
        typedef typename checked_iterator_category_helper<TIterator, __is_base_of(iterator_base_universal, TIterator)>::checked_category checked_category;
    };

    template<class TIterator>
    inline typename checked_iterator_category<TIterator>::checked_category checked_category(const TIterator&) {
        typename checked_iterator_category<TIterator>::checked_category category;
        return category;
    }

    template <class TIterator, bool inherits_from_iterator_base>
    class checked_iterator_base_helper2 {
    public:
        typedef unchanged_checked_iterator_base_type_tag checked_iterator_base_type;
    };

    template <class TIterator>
    class checked_iterator_base_helper2<TIterator, true> {
    public:
        typedef typename TIterator::checked_iterator_base_type checked_iterator_base_type;
    };

    template <class TIterator, class TBase>
    class checked_iterator_base_helper1 {
    public:
        typedef different_checked_iterator_base_type_tag base_type_tag;
        typedef TBase checked_iterator_base_type;
    };

    template <class TIterator>
    class checked_iterator_base_helper1<TIterator, unchanged_checked_iterator_base_type_tag> {
    public:
        typedef unchanged_checked_iterator_base_type_tag base_type_tag;
        typedef TIterator checked_iterator_base_type;
    };

    template <class TIterator>
    class checked_iterator_base_helper {
    public:
        typedef checked_iterator_base_helper2<TIterator, __is_base_of(_Iterator_base_universal, TIterator)> base_helper2;
        typedef checked_iterator_base_helper1<TIterator, typename base_helper2::checked_iterator_base_type > base_helper1;
        typedef typename base_helper1::base_type_tag checked_iterator_base_type_tag;
        typedef typename base_helper1::checked_iterator_base_type checked_iterator_base_type;
    };

    template<class TIterator, class TBase_tag>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(TIterator const& iterator, TBase_tag) {
        return iterator.checked_iterator_base();
    }

    template<class TIterator>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(const TIterator& iterator, unchanged_checked_iterator_base_type_tag) {
        return iterator;
    }

    template<class TIterator, class TBase_tag>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(TIterator& iterator, TBase_tag) {
        return iterator.checked_iterator_base();
    }

    template<class TIterator>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(TIterator& iterator, unchanged_checked_iterator_base_type_tag) {
        return iterator;
    }

    template<class TIterator>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(const TIterator& iterator) {
        typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type_tag base_tag;
        return checked_base(iterator, base_tag);
    }

    template<class TIterator>
    inline
    typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type
    __cdecl checked_base(TIterator& iterator) {
        typename checked_iterator_base_helper<TIterator>::checked_iterator_base_type_tag base_tag;
        return checked_base(iterator, base_tag);
    }

    template<class TDestinationIterator, class TBaseIterator>
    inline void __cdecl checked_assign_from_base(TDestinationIterator& destination, const TBaseIterator& source) {
        destination.checked_iterator_assign_from_base(source);
    }

    template<class TIterator>
    inline void __cdecl checked_assign_from_base(TIterator& destination, const TIterator& _Src) {
        destination = source;
    }

    template <class TValue>
    class move_operation_category {
    public:
        typedef undefinedmove_tag move_category;
    };

    template<class TIterator>
    inline
    typename move_operation_category<typename iterator_traits<TIterator>::value_type>::move_category
    move_category(const TIterator&) {
        typename move_operation_category<typename iterator_traits<TIterator>::value_type>::move_category category;
        return category;
    }

    template<class T1, class T2, class TCheckedCategory, class TCheckedCategory2>
    struct ptr_cat_with_checked_cat_helper {
        typedef nonscalar_ptr_iterator_tag ptr_category;
    };

    template<class T1, class T2>
    struct ptr_cat_helper {
        typedef typename ptr_cat_with_checked_cat_helper < T1, T2,
                typename checked_iterator_category<T1>::checked_category,
                typename checked_iterator_category<T2>::checked_category >::ptr_category ptr_category;
    };

    template<class T1, class T2, class TCheckedCategory1>
    struct ptr_cat_with_checked_cat_helper<T1, T2, TCheckedCategory1, range_checked_iterator_tag> {
        typedef typename ptr_cat_helper<T1, typename T2::inner_type>::ptr_category ptr_category;
    };

    template<class T1, class T2, class TCheckedCategory2>
    struct ptr_cat_with_checked_cat_helper<T1, T2, range_checked_iterator_tag, TCheckedCategory2> {
        typedef typename ptr_cat_helper<typename T1::inner_type, T2>::ptr_category ptr_category;
    };

    template<class T1, class T2>
    struct ptr_cat_with_checked_cat_helper<T1, T2, range_checked_iterator_tag, range_checked_iterator_tag> {
        typedef typename ptr_cat_helper<typename T1::inner_type, typename T2::inner_type>::ptr_category ptr_category;
    };

    template<class T1>
    struct ptr_cat_helper<T1, undefined_inner_type_tag> {
        typedef nonscalar_ptr_iterator_tag ptr_category;
    };

    template<class T2>
    struct ptr_cat_helper<undefined_inner_type_tag, T2> {
        typedef nonscalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<undefined_inner_type_tag, undefined_inner_type_tag> {
        typedef nonscalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<bool*, bool*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const bool*, bool*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<char*, char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const char*, char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<signed char*, signed char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const signed char*, signed char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<unsigned char*, unsigned char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const unsigned char*, unsigned char*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<wchar_t*, wchar_t*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const wchar_t*, wchar_t*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<short*, short*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const short*, short*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<unsigned short*, unsigned short*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const unsigned short*, unsigned short*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<int*, int*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const int*, int*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<unsigned int*, unsigned int*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const unsigned int*, unsigned int*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<long*, long*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const long*, long*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<unsigned long*, unsigned long*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const unsigned long*, unsigned long*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<float*, float*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const float*, float*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<double*, double*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct _Ptr_cat_helper<const double*, double*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };
    template<>
    struct ptr_cat_helper<long double*, long double*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const long double*, long double*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<__int64*, __int64*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const __int64*, __int64*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<unsigned __int64*, unsigned __int64*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<>
    struct ptr_cat_helper<const unsigned __int64*, unsigned __int64*> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<class T>
    struct ptr_cat_helper<T**, T**> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<class T>
    struct ptr_cat_helper<T**, const T**> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<class T>
    struct ptr_cat_helper<T* const*, T**> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<class T>
    struct ptr_cat_helper<T* const*, const T**> {
        typedef scalar_ptr_iterator_tag ptr_category;
    };

    template<class T1, class T2>
    inline typename ptr_cat_helper<T1, T2>::ptr_category
    __cdecl ptr_category(T1&, T2&) {
        typename ptr_cat_helper<T1, T2>::ptr_category category;
        return category;
    }

    template<class TInputIterator>
    inline void __cdecl debug_pointer(TInputIterator&, const wchar_t*, unsigned int) {
    }

    template<class T>
    inline void __cdecl debug_pointer(const T* first, const wchar_t* file, unsigned int line) {
        if (first == 0)
            debug_message(L"invalid null pointer", file, line);
    }

    template<class T>
    inline void __cdecl debug_pointer(T* first, const wchar_t* file, unsigned int line) {
        if (first == 0)
            debug_message(L"invalid null pointer", file, line);
    }

    template<class TInputIterator>
    inline void __cdecl debug_range2(TInputIterator, TInputIterator, const wchar_t*, unsigned int, input_iterator_tag) {
    }

    template<class TRandomIterator>
    inline void __cdecl debug_range2(TRandomIterator first, TRandomIterator last, const wchar_t* file, unsigned int line, random_access_iterator_tag) {
        if (first != last) {
            debug_pointer(first, file, line);
            debug_pointer(last, file, line);

            if (last < first)
                debug_message(L"invalid iterator range", file, line);
        }
    }

    template<class TInputIterator>
    inline void __cdecl debug_range(TInputIterator first, TInputIterator last, const wchar_t* file, unsigned int line) {
        debug_range2(first, last, file, line, iterator_category(first));
    }

    template<class TInputIterator>
    inline void __cdecl debug_order2(TInputIterator first, TInputIterator last, const wchar_t* file, unsigned int line, input_iterator_tag) {
    }

    template<class TForwardIterator>
    inline void __cdecl debug_order2(TForwardIterator first, TForwardIterator last, const wchar_t* file, unsigned int line, forward_iterator_tag) {
        if (first != last) {
            for (TForwardIterator next = first; ++next != last; ++first) {
                if (debug_lt(*next, *first, L"..\\VC\\ce\\include\\xutility", 1590)) {
                    debug_message(L"sequence not ordered", file, line);
                }
            }
        }
    }

    template<class TInputIterator>
    inline void __cdecl debug_order(TInputIterator first, TInputIterator last, const wchar_t* file, unsigned int line) {
        debug_range(first, last, file, line);
        debug_order2(first, last, file, line, iterator_category(first));
    }

    template<class TInputIterator, class TPredication>
    inline void __cdecl debug_order2(TInputIterator first, TInputIterator last, TPredication predication, const wchar_t* file, unsigned int line, input_iterator_tag) {
    }

    template<class TForwardIterator, class TPredication>
    inline void __cdecl debug_order2(TForwardIterator first, TForwardIterator last, TPredication predication, const wchar_t* file, unsigned int line, forward_iterator_tag) {
        if (first != last) {
            for (TForwardIterator next = first; ++next != last; ++first) {
                if (debug_lt_pred(predication, *next, *first, L"..\\VC\\ce\\include\\xutility", 1617)) {
                    debug_message(L"sequence not ordered", file, line);
                }
            }
        }
    }

    template<class TInputIterator, class TPredication>
    inline void __cdecl debug_order(TInputIterator first, TInputIterator last, TPredication predication, const wchar_t* file, unsigned int line) {
        debug_range(first, last, file, line);
        debug_pointer(predication, file, line);
        debug_order2(first, last, pred, file, line, iterator_category(first));
    }

    template<class TInputIterator>
    inline void __cdecl debug_order_single2(TInputIterator first, TInputIterator last, bool isFirstIteration, const wchar_t* file, unsigned int line, input_iterator_tag) {
    }

    template<class TForwardIterator>
    inline void __cdecl debug_order_single2(TForwardIterator first, TForwardIterator last, bool, const wchar_t* file, unsigned int line, forward_iterator_tag) {
        if (first != last) {
            TForwardIterator next = first;

            if (++next != last) {
                if (debug_lt(*next, *first, L"..\\VC\\ce\\include\\xutility", 1646)) {
                    debug_message(L"sequence not ordered", file, line);
                }
            }
        }
    }

    template<class TInputIterator>
    inline void __cdecl debug_order_single(TInputIterator first, TInputIterator last, bool isFirstIteration, const wchar_t* file, unsigned int line) {
        debug_order_single2(first, last, isFirstIteration, file, line, iterator_category(first));
    }

    template<class TInputIterator, class TPredication>
    inline void __cdecl debug_order_single2(TInputIterator first, TInputIterator last, TPredication predication, bool isFirstIteration, const wchar_t* file, unsigned int line, input_iterator_tag) {
    }

    template<class TForwardIterator, class TPredication>
    inline void __cdecl debug_order_single2(TForwardIterator first, TForwardIterator last, TPredication predication, bool isFirstIteration, const wchar_t* file, unsigned int line, forward_iterator_tag) {
        if (first != last) {
            TForwardIterator next = first;

            if (++next != last) {
                if (debug_lt_pred(predication, *next, *first, L"..\\VC\\ce\\include\\xutility", 1675)) {
                    debug_message(L"sequence not ordered", file, line);
                }
            }
        }
    }

    template<class TInputIterator, class TPredication>
    inline void __cdecl debug_order_single(TInputIterator first, TInputIterator last, TPredication predication, bool isFirstIteration, const wchar_t* file, unsigned int line) {
        debug_order_single2(first, last, pred, isFirstIteration, file, line, iterator_category(first));
    }

    template<class TIterator>
    inline typename iterator_traits<TIterator>::value_type* __cdecl value_type(TIterator) {
        return (0);
    }

    template<class TInputIterator, class TDifference>
    inline void __cdecl advance(TInputIterator& where, TDifference offset) {
        _advance(where, offset, iterator_category(where));
    }

    template<class TInputIterator, class TDifference>
    inline void __cdecl _advance(TInputIterator& where, TDifference offset, input_iterator_tag) {
        for (; 0 < offset; --offset) {
            ++where;
        }
    }

    template<class TForwardIterator, class TDifference>
    inline void __cdecl _advance(TForwardIterator& where, TDifference offset, forward_iterator_tag) {
        for (; 0 < offset; --offset) {
            ++where;
        }
    }

#pragma warning(push)
#pragma warning(disable: 6295)

    template<class TBidirectionalIterator, class TDifference>
    inline void __cdecl _advance(TBidirectionalIterator& where, TDifference offset, bidirectional_iterator_tag) {
        for (; 0 < offset; --offset) {
            ++where;
        }

        for (; offset < 0; ++offset) {
            --where;
        }
    }

#pragma warning(pop)

    template<class TRandomIterator, class TDifference>
    inline void __cdecl _Advance(TRandomIterator& where, TDifference offset, random_access_iterator_tag) {
        where += offset;
    }

    template<class TIterator>
    inline typename iterator_traits<TIterator>::difference_type* __cdecl distance_type(TIterator) {
        return (0);
    }

    template<class TInputIterator, class TDifference>
    inline void __cdecl _distance2(TInputIterator first, TInputIterator last, TDifference& offset, input_iterator_tag) {
        for (; first != last; ++first) {
            ++offset;
        }
    }

    template<class TForwardIterator, class TDifference>
    inline void __cdecl _distance2(TForwardIterator first, TForwardIterator last, TDifference& offset, forward_iterator_tag) {
        for (; first != last; ++first)
            ++offset;
    }

    template<class TBidirectionalIterator, class TDifference>
    inline void __cdecl _distance2(TBidirectionalIterator first, TBidirectionalIterator last, TDifference& offset, bidirectional_iterator_tag) {
        for (; first != last; ++first) {
            ++offset;
        }
    }

    template<class TRandomIterator, class TDifference>
    inline void __cdecl _distance2(TRandomIterator first, TRandomIterator last, TDifference& offset, random_access_iterator_tag) {
        if (first != last) {
            debug_pointer(first, L"..\\VC\\ce\\include\\xutility", 1847);
            debug_pointer(last, L"..\\VC\\ce\\include\\xutility", 1848);
        }

        offset += last - first;
    }

    template<class TInputIterator>
    inline typename iterator_traits<TInputIterator>::difference_type __cdecl distance(TInputIterator first, TInputIterator last) {
        typename iterator_traits<TInputIterator>::difference_type offset = 0;
        _distance2(first, last, offset, iterator_category(first));
        return offset;
    }

    template<class TInputIterator, class TDifference>
    inline void __cdecl _distance(TInputIterator first, TInputIterator last, TDifference& offset) {
        _distance2(first, last, offset, iterator_category(first));
    }

    template<class TRandomIterator, class TBase>
    class ReverseRandomIterator : public TBase {
    public:
        typedef ReverseRandomIterator<TRandomIterator, TBase> MyType;
        typedef typename iterator_traits<TRandomIterator>::iterator_category iterator_category;
        typedef typename iterator_traits<TRandomIterator>::value_type value_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type difference_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type distance_type;
        typedef typename iterator_traits<TRandomIterator>::pointer pointer;
        typedef typename iterator_traits<TRandomIterator>::reference reference;
        typedef TRandomIterator iterator_type;

        ReverseRandomIterator() {
        }

        explicit ReverseRandomIterator(TRandomIterator right) : current_(right) {
        }

        template<class TRandomIterator2, class TBase2>
        ReverseRandomIterator(const ReverseRandomIterator<TRandomIterator2, TBase2>& right) : current_(right.base()) {
        }

        TRandomIterator base() const {
            return current_;
        }

        reference operator*() const {
            TRandomIterator temp = current_;
            return *--temp;
        }

        pointer operator->() const {
            return &** this;
        }

        MyType& operator++() {
            --current_;
            return *this;
        }

        MyType operator++(int) {
            MyType temp = *this;
            --current_;
            return temp;
        }

        MyType&  operator--() {
            ++current_;
            return *this;
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current_;
            return temp;
        }

        template<class TRandomIterator2, class TBase2>
        bool  equal(const ReverseRandomIterator<TRandomIterator2, TBase2>& right) const {
            return current_ == right.base();
        }

        MyType&  operator+=(difference_type offset) {
            current_ -= offset;
            return (*this);
        }

        MyType  operator+(difference_type offset) const {
            return MyType(current_ - offset);
        }

        MyType&  operator-=(difference_type offset) {
            current_ += offset;
            return *this;
        }

        MyType operator-(difference_type offset) const {
            return MyType(current_ + offset);
        }

        reference  operator[](difference_type offset) const {
            return *(*this + offset);
        }

        template<class TRandomIterator2, class TBase2>
        bool less(const ReverseRandomIterator<TRandomIterator2, TBase2>& right) const {
            return right.base() < current_;
        }

        template<class TRandomIterator2, class TBase2>
        difference_type minus(const ReverseRandomIterator<TRandomIterator2, TBase2>& right) const {
            return right.base() - current_;
        }
    protected:
        TRandomIterator current_;
    };

    template<class TRandomIterator, class TBase, class TDifference>
    inline
    ReverseRandomIterator<TRandomIterator, TBase>
    operator+(TDifference offset, const ReverseRandomIterator<TRandomIterator, TBase>& right) {
        return right + offset;
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    typename TBase1::difference_type
    operator-(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return (left.minus(right));
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator==(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return left.equal(right);
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator!=(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return !(left == right);
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator<(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return left.less(right);
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator>(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return right < left;
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator<=(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return !(right < left);
    }

    template<class TRandomIterator1, class TBase1, class TRandomIterator2, class TBase2>
    inline
    bool operator>=(const ReverseRandomIterator<TRandomIterator1, TBase1>& left, const ReverseRandomIterator<TRandomIterator2, TBase2>& right) {
        return !(left < right);
    }

    template<class TRandomIterator>
    class reverse_iterator
        : public ReverseRandomIterator < TRandomIterator, iterator <
        typename iterator_traits<TRandomIterator>::iterator_category,
        typename iterator_traits<TRandomIterator>::value_type,
        typename iterator_traits<TRandomIterator>::difference_type,
        typename iterator_traits<TRandomIterator>::pointer,
            typename iterator_traits<TRandomIterator>::reference > > {
        typedef reverse_iterator<TRandomIterator> MyType;
        typedef ReverseRandomIterator < TRandomIterator, iterator <
        typename iterator_traits<TRandomIterator>::iterator_category,
                 typename iterator_traits<TRandomIterator>::value_type,
                 typename iterator_traits<TRandomIterator>::difference_type,
                 typename iterator_traits<TRandomIterator>::pointer,
                 typename iterator_traits<TRandomIterator>::reference > > MyBaseType;
    public:
        typedef typename iterator_traits<TRandomIterator>::iterator_category iterator_category;
        typedef typename iterator_traits<TRandomIterator>::value_type value_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type difference_type;
        typedef typename iterator_traits<TRandomIterator>::difference_type distance_type;
        typedef typename iterator_traits<TRandomIterator>::pointer pointer;
        typedef typename iterator_traits<TRandomIterator>::reference reference;
        typedef TRandomIterator iterator_type;

        reverse_iterator() {
        }

        explicit  reverse_iterator(TRandomIterator right)
            : MyBaseType(right) {
        }

        template<class TOther>
        reverse_iterator(const reverse_iterator<TOther>& right)
            : MyBaseType(right.base()) {
        }

        reverse_iterator(MyBaseType right)
            : MyBaseType(right) {
        }

        typedef typename ::std::checked_iterator_category<TRandomIterator>::checked_category checked_iterator_category;
        typedef reverse_iterator<typename checked_iterator_base_helper<TRandomIterator>::checked_iterator_base_type> checked_iterator_base_type;

        checked_iterator_base_type checked_iterator_base() const {
            typename checked_iterator_base_type base(::std::checked_base(this->current));
            return base;
        }

        void checked_iterator_assign_from_base(checked_iterator_base_type base) {
            ::std::checked_assign_from_base(this->current, base.base());
        }

        MyType& operator++() {
            ++*((MyBaseType*)this);
            return (*this);
        }

        MyType operator++(int) {
            MyType temp = *this;
            ++*this;
            return (temp);
        }

        MyType& operator--() {
            --*((MyBaseType*)this);
            return (*this);
        }

        MyType operator--(int) {
            MyType temp = *this;
            --*this;
            return (temp);
        }

        MyType& operator+=(difference_type offset) {
            *((MyBaseType*)this) += offset;
            return (*this);
        }

        MyType operator+(difference_type offset) const {
            MyType temp = *this;
            return (temp += offset);
        }

        MyType& operator-=(difference_type offset) {
            *((MyBaseType*)this) -= offset;
            return (*this);
        }

        MyType operator-(difference_type offset) const {
            MyType temp = *this;
            return (temp -= offset);
        }
    };

    template<class TRandomIterator, class TDifference>
    inline reverse_iterator<TRandomIterator> __cdecl operator+(TDifference offset, const reverse_iterator<TRandomIterator>& right) {
        return (right + offset);
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    typename reverse_iterator<TRandomIterator1>::difference_type
    __cdecl operator-(const reverse_iterator<TRandomIterator1>& left,
                      const reverse_iterator<TRandomIterator2>& right) {
        return (left.minus(right));
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator==(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (left.equal(right));
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator!=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(left == right));
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator<(const reverse_iterator<TRandomIterator1>& left,
                           const reverse_iterator<TRandomIterator2>& right) {
        return (left.less(right));
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator>(const reverse_iterator<TRandomIterator1>& left,
                           const reverse_iterator<TRandomIterator2>& right) {
        return (right < left);
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator<=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(right < left));
    }

    template<class TRandomIterator1, class TRandomIterator2> inline
    bool __cdecl operator>=(const reverse_iterator<TRandomIterator1>& left,
                            const reverse_iterator<TRandomIterator2>& right) {
        return (!(left < right));
    }

    template < class TBidirectionIterator, class T, class TReference = T&, class TPointer = T*, class TDifference = ptrdiff_t >
    class reverse_bidirectional_iterator : public iterator<bidirectional_iterator_tag, T, TDifference, TPointer, TReference> {
    public:
        typedef reverse_bidirectional_iterator<TBidirectionIterator, T, TReference, TPointer, TDifference> MyType;
        typedef TBidirectionIterator iterator_type;

        reverse_bidirectional_iterator() {
        }

        explicit reverse_bidirectional_iterator(TBidirectionIterator right)
            : current_(right) {
        }

        TBidirectionIterator base() const {
            return (current_);
        }

        TReference operator*() const {
            TBidirectionIterator temp = current_;
            return (*--temp);
        }

        TPointer  operator->() const {
            TReference temp = **this;
            return (&temp);
        }

        MyType&  operator++() {
            --current_;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            --current_;
            return (temp);
        }

        MyType&  operator--() {
            ++current_;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current_;
            return (temp);
        }

        bool  operator==(const MyType& right) const {
            return (current_ == right.current_);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }
    protected:
        TBidirectionIterator current_;
    };

    template < class TBidirectionIterator, class TBidirectionIterator2 = TBidirectionIterator >
    class ReverseBidirectionIterator
        : public iterator <
        typename iterator_traits<TBidirectionIterator>::iterator_category,
        typename iterator_traits<TBidirectionIterator>::value_type,
        typename iterator_traits<TBidirectionIterator>::difference_type,
        typename iterator_traits<TBidirectionIterator>::pointer,
            typename iterator_traits<TBidirectionIterator>::reference > {
    public:
        typedef ReverseBidirectionIterator<TBidirectionIterator, TBidirectionIterator2> MyType;
        typedef typename iterator_traits<TBidirectionIterator>::difference_type difference;
        typedef typename iterator_traits<TBidirectionIterator>::pointer pointer;
        typedef typename iterator_traits<TBidirectionIterator>::reference reference;
        typedef TBidirectionIterator iterator_type;

        ReverseBidirectionIterator() {
        }

        explicit  ReverseBidirectionIterator(TBidirectionIterator right)
            : current_(right) {
        }

        ReverseBidirectionIterator(const ReverseBidirectionIterator<TBidirectionIterator2>& other)
            : current_(other.base()) {
        }

        TBidirectionIterator  base() const {
            return (current_);
        }

        reference  operator*() const {
            TBidirectionIterator temp = current_;
            return (*--temp);
        }

        pointer  operator->() const {
            reference temp = **this;
            return (&temp);
        }

        MyType&  operator++() {
            --current_;
            return (*this);
        }

        MyType  operator++(int) {
            MyType temp = *this;
            --current_;
            return (temp);
        }

        MyType&  operator--() {
            ++current_;
            return (*this);
        }

        MyType  operator--(int) {
            MyType temp = *this;
            ++current_;
            return (temp);
        }

        bool  operator==(const MyType& right) const {
            return (current_ == right.current_);
        }

        bool  operator!=(const MyType& right) const {
            return (!(*this == right));
        }
    protected:
        TBidirectionIterator current_;
    };

    template<class TInputIterator, class TOutputIterator, class TIteratorCategory>
    inline
    TOutputIterator __cdecl copy_opt(TInputIterator first, TInputIterator last, TOutputIterator destination,
                                     TIteratorCategory, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\VC\\ce\\include\\xutility", 2446);

        for (; first != last; ++destination, ++first)
            *destination = *first;

        return (destination);
    }

    template<class TInputIterator, class TOutputIterator>
    inline
    TOutputIterator __cdecl copy_opt(TInputIterator first, TInputIterator last, TOutputIterator destination,
                                     random_access_iterator_tag, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
        TOutputIterator result = destination + (last - first);
        copy_opt(first, last, ::std::checked_base(destination),
                 forward_iterator_tag(), nonscalar_ptr_iterator_tag(), range_checked_iterator_tag());
        return result;
    }

    template<class TInputIterator, class TOutputIterator, class TIteratorCategory>
    inline
    TOutputIterator __cdecl copy_opt(TInputIterator first, TInputIterator last, TOutputIterator destination,
                                     TIteratorCategory, scalar_ptr_iterator_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\VC\\ce\\include\\xutility", 2473);

        if (first != last)
            debug_pointer(destination, L"..\\VC\\ce\\include\\xutility", 2475);

        ptrdiff_t offset = last - first;
        TOutputIterator result = destination + offset;

        if (offset > 0)
            ::memmove_s((&*destination), (offset * sizeof(*first)), (&*first), (offset * sizeof(*first)));

        return result;
    }

    template <bool Condition, class TResult>
    struct enable_if;

    template <class TResult>
    struct enable_if<true, TResult> {
        typedef TResult result;
    };

    template <class TCheckedTiteratorCategory>
    struct is_checked_iterator_helper {
        enum { result = false };
    };

    template <>
    struct is_checked_iterator_helper<range_checked_iterator_tag> {
        enum { result = true };
    };

    template <class TIterator>
    struct is_checked_iterator {
        enum { result =
                   is_checked_iterator_helper<typename checked_iterator_category<TIterator>::checked_category>::result
             };
    };

    template<class TInputIterator, class TOutputIterator>
    inline typename ::std::enable_if< ::std::is_checked_iterator< TOutputIterator >::result, TOutputIterator >::result
    __cdecl copy(TInputIterator first, TInputIterator last, TOutputIterator destination) {
        return (copy_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                         iterator_random(first, destination), ptr_category(first, destination), range_checked_iterator_tag()));
    }

    template<class TInputInterator, class TOutElement, size_t size>
    inline
    TOutElement* __cdecl copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
        return copy(::std::checked_base(first), ::std::checked_base(last), ::stdext::make_checked_array_iterator(destination, size)).base();
    }

    template<class TInputInterator, class TOutputInterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result __cdecl copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
        return (copy_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                         iterator_random(first, destination), ptr_category(first, destination), range_checked_iterator_tag()));
    }

    template<class TInputInterator, class TOutputInterator, class TIteratorCategory, class TMoveCategory>
    inline
    TOutputInterator __cdecl move_opt(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                      TIteratorCategory first_destination_category, TMoveCategory, range_checked_iterator_tag) {
        return copy_opt(first, last, destination,
                        first_destination_category, ptr_category(first, destination), range_checked_iterator_tag());
    }

    template<class TInputInterator, class TOutputInterator, class TIteratorCategory>
    inline
    TOutputInterator __cdecl move_opt(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                      TIteratorCategory, swapmove_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\VC\\ce\\include\\xutility", 2585);

        for (; first != last; ++destination, ++first)
            ::std::swap(*destination, *first);

        return (destination);
    }

    template<class TInputInterator, class TOutputInterator, class TIteratorCategory>
    inline
    TOutputInterator __cdecl move_opt(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                      random_access_iterator_tag, swapmove_tag move_category, range_checked_iterator_tag) {
        TOutputInterator result = destination + (last - first);
        move_opt(first, last, ::std::checked_base(destination),
                 forward_iterator_tag(), move_category, range_checked_iterator_tag());
        return result;
    }

    template<class TInputInterator, class TOutputInterator>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
    __cdecl move(TInputInterator first, TInputInterator last, TOutputInterator destination) {
        return move_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                        iterator_random(first, destination), move_category(destination), ::std::range_checked_iterator_tag());
    }

    template<class TInputInterator, class TOutElement, size_t size>
    inline
    TOutElement* __cdecl move(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
        return move(::std::checked_base(first), ::std::checked_base(last),
                    ::stdext::make_checked_array_iterator(destination, size)).base();
    }

    template<class TInputInterator, class TOutputInterator>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
    __cdecl move(TInputInterator first, TInputInterator last, TOutputInterator destination) {
        return move_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                        iterator_random(first, destination), move_category(destination), ::std::range_checked_iterator_tag());
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2, class TIteratorCategory>
    inline
    TBidirectionIterator2 __cdecl copy_backward_opt(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination,
            TIteratorCategory, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\VC\\ce\\include\\xutility", 2650);

        while (first != last)
            *--destination = *--last;

        return (destination);
    }

    template<class TInputInterator, class TOutputInterator>
    inline
    TOutputInterator __cdecl copy_backward_opt(TInputInterator first, TInputInterator last, TOutputInterator destination,
            random_access_iterator_tag, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
        TOutputInterator result = destination - (last - first);
        copy_backward_opt(first, last, ::std::checked_base(destination),
                          forward_iterator_tag(), nonscalar_ptr_iterator_tag(), range_checked_iterator_tag());
        return result;
    }

    template<class TInputInterator, class TOutputInterator, class TIteratorCategory>
    inline
    TOutputInterator __cdecl copy_backward_opt(TInputInterator first, TInputInterator last, TOutputInterator destination,
            TIteratorCategory, scalar_ptr_iterator_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\VC\\ce\\include\\xutility", 2677);

        if (first != last)
            debug_pointer(destination, L"..\\VC\\ce\\include\\xutility", 2679);

        ptrdiff_t offset = last - first;
        TOutputInterator result = destination - offset;

        if (offset > 0)
            ::memmove_s((&*result), (offset * sizeof(*first)), (&*first), (offset * sizeof(*first)));

        return result;
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2> inline
    typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
    __cdecl copy_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
        return copy_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                 iterator_random(first, destination), ptr_category(first, destination), ::std::range_checked_iterator_tag());
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2> inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
    __cdecl copy_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
        return copy_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                 iterator_random(first, destination), ptr_category(first, destination), ::std::range_checked_iterator_tag());
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2, class TIteratorCategory, class TMoveCategory>
    inline
    TBidirectionIterator2 __cdecl move_backward_opt(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination,
            TIteratorCategory first_destination_category, TMoveCategory, range_checked_iterator_tag) {
        return copy_backward_opt(first, last, destination,
                                 first_destination_category, ptr_category(first, destination), range_checked_iterator_tag());
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2, class TIteratorCategory>
    inline
    TBidirectionIterator2 __cdecl move_backward_opt(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination,
            TIteratorCategory, swapmove_tag, range_checked_iterator_tag) {
        debug_range(first, last, L"..\\ce\\include\\xutility", 2738);

        while (first != last)
            ::std::swap(*--destination, *--last);

        return (destination);
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2>
    inline
    TBidirectionIterator2 __cdecl move_backward_opt(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination,
            random_access_iterator_tag, swapmove_tag move_category, range_checked_iterator_tag) {
        TBidirectionIterator2 result = destination - (last - first);
        move_backward_opt(first, last, ::std::checked_base(destination),
                          forward_iterator_tag(), move_category, range_checked_iterator_tag());
        return result;
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
    __cdecl move_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
        return move_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                 iterator_random(first, destination), move_category(destination), ::std::range_checked_iterator_tag());
    }

    template<class TBidirectionIterator1, class TBidirectionIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
    __cdecl move_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
        return move_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                 iterator_random(first, destination), move_category(destination), ::std::range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2, class TInputIteratorCategory>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                     TInputIteratorCategory, range_checked_iterator_tag) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 2798);

        if (first1 != last1)
            debug_pointer(first2, L"..\\ce\\include\\xutility", 2800);

        for (; first1 != last1 && *first1 == *first2;)
            ++first1, ++first2;

        return (pair<TInputIterator1, TInputIterator2>(first1, first2));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                     random_access_iterator_tag, range_checked_iterator_tag) {
        TInputIterator2 last2 = first2 + (last1 - first1);
        (last2);
        pair<TInputIterator1, typename ::std::checked_iterator_base_helper<TInputIterator2>::checked_iterator_base_type> result =
            mismatch(first1, last1, ::std::checked_base(first2),
                     forward_iterator_tag(), range_checked_iterator_tag());
        ::std::checked_assign_from_base(first2, result.second);
        return (pair<TInputIterator1, TInputIterator2>(result.first, first2));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
        pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
            mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        ::std::checked_assign_from_base(first1, result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, result.second));
    }

    template<class TInputIterator1, class TInElement2, size_t size>
    inline
    pair<TInputIterator1, TInElement2*>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size]) {
        pair<TInputIterator1, ::stdext::checked_array_iterator<TInElement2*> > result =
            mismatch(first1, last1, ::stdext::make_checked_array_iterator(first2, size));
        return (pair<TInputIterator1, TInElement2*>(result.first, result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
        pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
            mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        ::std::checked_assign_from_base(first1, result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication, class TInputIteratorCategory>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication,
                     TInputIteratorCategory, range_checked_iterator_tag) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 2884);

        if (first1 != last1)
            debug_pointer(first2, L"..\\ce\\include\\xutility", 2886);

        debug_pointer(predication, L"..\\ce\\include\\xutility", 2887);

        for (; first1 != last1 && predication(*first1, *first2);)
            ++first1, ++first2;

        return (pair<TInputIterator1, TInputIterator2>(first1, first2));
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    pair<TInputIterator1, TInputIterator2>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication,
                     random_access_iterator_tag, range_checked_iterator_tag) {
        TInputIterator2 last2 = first2 + (last1 - first1);
        (last2);
        pair<TInputIterator1, typename ::std::checked_iterator_base_helper<TInputIterator2>::checked_iterator_base_type> result =
            mismatch(first1, last1, ::std::checked_base(first2), predication,
                     forward_iterator_tag(), range_checked_iterator_tag());
        ::std::checked_assign_from_base(first2, result.second);
        return (pair<TInputIterator1, TInputIterator2>(result.first, first2));
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
        pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
            mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        ::std::checked_assign_from_base(first1, result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, result.second));
    }

    template<class TInputIterator1, class TInElement2, class TPredication, size_t size>
    inline
    pair<TInputIterator1, TInElement2*>
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size], TPredication predication) {
        pair<TInputIterator1, ::stdext::checked_array_iterator<TInElement2*> > result =
            mismatch(first1, last1, ::stdext::make_checked_array_iterator(first2, size), predication);
        return (pair<TInputIterator1, TInElement2*>(result.first, result.second.base()));
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
    __cdecl mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
        pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
            mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        ::std::checked_assign_from_base(first1, result.first);
        return (pair<TInputIterator1, TInputIterator2>(first1, result.second));
    }

    template<class TInputIterator1, class TInputIterator2, class TInputIteratorCategory>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                       TInputIteratorCategory, range_checked_iterator_tag) {
        for (; first1 != last1; ++first1, ++first2)
            if (!(*first1 == *first2))
                return (false);

        return (true);
    }

    inline bool __cdecl equal(const char* first1, const char* last1, const char* first2,
                              random_access_iterator_tag, range_checked_iterator_tag) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 2979);

        if (first1 != last1)
            debug_pointer(first2, L"..\\ce\\include\\xutility", 2981);

        return (::memcmp(first1, first2, last1 - first1) == 0);
    }

    inline bool __cdecl equal(const signed char* first1, const signed char* last1, const signed char* first2,
                              random_access_iterator_tag, range_checked_iterator_tag) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 2992);

        if (first1 != last1)
            debug_pointer(first2, L"..\\ce\\include\\xutility", 2994);

        return (::memcmp(first1, first2, last1 - first1) == 0);
    }

    inline bool __cdecl equal(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2,
                              random_access_iterator_tag, range_checked_iterator_tag) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 3005);

        if (first1 != last1)
            debug_pointer(first2, L"..\\ce\\include\\xutility", 3007);

        return (::memcmp(first1, first2, last1 - first1) == 0);
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                       random_access_iterator_tag, range_checked_iterator_tag) {
        TInputIterator2 last2 = first2 + (last1 - first1);
        (last2);
        return equal(first1, last1, ::std::checked_base(first2),
                     forward_iterator_tag(), range_checked_iterator_tag());
    }
}

namespace std {

    template<class TInputIterator1, class TInputIterator2>
    inline typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
    __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
        return equal(::std::checked_base(first1), ::std::checked_base(last1), first2,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, size_t size>
    inline bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size]) {
        return equal(first1, last1,
                     ::stdext::make_checked_array_iterator(first2, size));
    }

    template<class TInputIterator1, class TInputIterator2>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
    __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
        return equal(::std::checked_base(first1), ::std::checked_base(last1), first2,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication, class TInputIteratorCategory>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication,
                       TInputIteratorCategory, range_checked_iterator_tag) {
        for (; first1 != last1; ++first1, ++first2)
            if (!predication(*first1, *first2))
                return (false);

        return (true);
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication,
                       random_access_iterator_tag, range_checked_iterator_tag) {
        TInputIterator2 last2 = first2 + (last1 - first1);
        (last2);
        return equal(first1, last1, ::std::checked_base(first2), predication,
                     forward_iterator_tag(), range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
    __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
        return equal(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInElement2, class TPredication, size_t size>
    inline
    bool __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size], TPredication predication) {
        return equal(first1, last1,
                     ::stdext::make_checked_array_iterator(first2, size), predication);
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
    __cdecl equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
        return equal(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                     iterator_random(first1, first2), ::std::range_checked_iterator_tag());
    }

    template<class TForwardIterator, class T> inline
    void __cdecl fill(TForwardIterator first, TForwardIterator last, const T& value) {
        debug_range(first, last, L"..\\ce\\include\\xutility", 3133);

        for (; first != last; ++first)
            *first = value;
    }

    inline void __cdecl fill([SA_Pre(Null=SA_No, WritableElements="\n@""last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] char* first,
                             [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] char* last, int value) {
        debug_range(first, last, L"..\\ce\\include\\xutility", 3141);
        ::memset(first, value, last - first);
    }

    inline void __cdecl fill([SA_Pre(Null=SA_No, WritableElements="\n@""last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] signed char* first,
                             [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] signed char* last, int value) {
        debug_range(first, last, L"..\\ce\\include\\xutility", 3148);
        ::memset(first, value, last - first);
    }

    inline void __cdecl fill(
        [SA_Pre(Null=SA_No, WritableElements="\n@""last-first")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="\n@""last-first")] [SA_Post(Deref=1, Valid=SA_Yes)] unsigned char* first,
        [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1, Valid=SA_Yes)] [SA_Pre(Deref=1, Access=SA_Read)] unsigned char* last, int value) {
        debug_range(first, last, L"..\\ce\\include\\xutility", 3156);
        ::memset(first, value, last - first);
    }

    template<class TForwardIterator, class T> inline
    void __cdecl fill(TForwardIterator first, TForwardIterator last, const T& value) {
        fill(::std::checked_base(first), ::std::checked_base(last), value);
    }

    template<class TOutputInterator, class TDifference, class T> inline
    void __cdecl fill_n(TOutputInterator first, TDifference count, const T& value,
                        range_checked_iterator_tag) {
        for (; 0 < count; --count, ++first)
            *first = value;
    }

    inline void __cdecl fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] char* first,
                               size_t count, int value, range_checked_iterator_tag) {
        if (0 < count)
            debug_pointer(first, L"..\\ce\\include\\xutility", 3183);

        ::memset(first, value, count);
    }

    inline void __cdecl fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] signed char* first,
                               size_t count, int value, range_checked_iterator_tag) {
        if (0 < count)
            debug_pointer(first, L"..\\ce\\include\\xutility", 3195);

        ::memset(first, value, count);
    }

    inline void __cdecl fill_n([SA_Pre(Null=SA_Maybe, WritableElements="count")] [SA_Pre(Deref=1, Valid=SA_No)] [SA_Post(ValidElements="count")] [SA_Post(Deref=1, Valid=SA_Yes)] unsigned char* first,
                               size_t count, int value, range_checked_iterator_tag) {
        if (0 < count)
            debug_pointer(first, L"..\\ce\\include\\xutility", 3207);

        ::memset(first, value, count);
    }

    template<class TOutputInterator, class TDifference, class T, class TOutputCategory>
    inline
    void __cdecl fill_n(TOutputInterator first, TDifference count, const T& value,
                        TOutputCategory, range_checked_iterator_tag) {
        fill_n(first, count, value, range_checked_iterator_tag());
    }

    template<class TOutputInterator, class TDifference, class T>
    inline
    void __cdecl fill_n(TOutputInterator first, TDifference count, const T& value,
                        random_access_iterator_tag, range_checked_iterator_tag) {
        TOutputInterator last = first + count;
        (last);
        fill_n(::std::checked_base(first), count, value, range_checked_iterator_tag());
    }

    template<class TOutputInterator, class TDifference, class T>
    inline
    typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
    __cdecl fill_n(TOutputInterator first, TDifference count, const T& value) {
        fill_n(first, count, value, iterator_category(first), ::std::range_checked_iterator_tag());
    }

    template<class TOutElement, class TDifference, class T, size_t size>
    inline
    void __cdecl fill_n(TOutElement(&first)[size], TDifference count, const T& value) {
        fill_n(::stdext::make_checked_array_iterator(first, size), count, value);
    }

    template<class TOutputInterator, class TDifference, class T>
    inline
    __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
    typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
    __cdecl fill_n(TOutputInterator first, TDifference count, const T& value) {
        fill_n(first, count, value, iterator_category(first), ::std::range_checked_iterator_tag());
    }

    template<class TInputIterator1, class TInputIterator2> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 3281);
        debug_range(first2, last2, L"..\\ce\\include\\xutility", 3282);

        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            if (debug_lt(*first1, *first2, L"..\\ce\\include\\xutility", 3284))
                return (true);
            else if (*first2 < *first1)
                return (false);

        return (first1 == last1 && first2 != last2);
    }

    template<class TInputIterator1, class TInputIterator2> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2) {
        return lexicographical_compare(::std::checked_base(first1), ::std::checked_base(last1),
                                       ::std::checked_base(first2), ::std::checked_base(last2));
    }

    inline bool __cdecl lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
            const unsigned char* first2, const unsigned char* last2) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 3303);
        debug_range(first2, last2, L"..\\ce\\include\\xutility", 3304);
        ptrdiff_t num1 = last1 - first1;
        ptrdiff_t num2 = last2 - first2;
        int ans = ::memcmp(first1, first2, num1 < num2 ? num1 : num2);
        return (ans < 0 || ans == 0 && num1 < num2);
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2, TPredication predication) {
        debug_range(first1, last1, L"..\\ce\\include\\xutility", 3332);
        debug_range(first2, last2, L"..\\ce\\include\\xutility", 3333);
        debug_pointer(predication, L"..\\ce\\include\\xutility", 3334);

        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\xutility", 3336))
                return (true);
            else if (predication(*first2, *first1))
                return (false);

        return (first1 == last1 && first2 != last2);
    }

    template<class TInputIterator1, class TInputIterator2, class TPredication> inline
    bool __cdecl lexicographical_compare(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2, TPredication predication) {
        return lexicographical_compare(::std::checked_base(first1), ::std::checked_base(last1),
                                       ::std::checked_base(first2), ::std::checked_base(last2), predication);
    }

    template<class T> inline
    const T& (__cdecl max)(const T& left, const T& right) {
        return (debug_lt(left, right, L"..\\ce\\include\\xutility", 3360) ? right : left);
    }

    template<class T, class TPredication> inline
    const T& (__cdecl max)(const T& left, const T& right, TPredication predication) {
        return (debug_lt_pred(predication, left, right, L"..\\ce\\include\\xutility", 3368) ? right : left);
    }

    template<class T> inline
    const T& (__cdecl min)(const T& left, const T& right) {
        return (debug_lt(right, left, L"..\\ce\\include\\xutility", 3375) ? right : left);
    }

    template<class T, class TPredication> inline
    const T& (__cdecl min)(const T& left, const T& right, TPredication predication) {
        return (debug_lt_pred(predication, right, left, L"..\\ce\\include\\xutility", 3383) ? right : left);
    }
}

namespace stdext {
    template <class TIterator>
    class checked_array_iterator
        : public ::std::iterator <
        typename ::std::iterator_traits<TIterator>::iterator_category,
        typename ::std::iterator_traits<TIterator>::value_type,
        typename ::std::iterator_traits<TIterator>::difference_type,
        typename ::std::iterator_traits<TIterator>::pointer,
            typename ::std::iterator_traits<TIterator>::reference > {
    public:
        typedef checked_array_iterator<TIterator> MyType;
        typedef typename ::std::iterator_traits<TIterator>::difference_type difference_type;
        typedef typename ::std::iterator_traits<TIterator>::pointer pointer;
        typedef typename ::std::iterator_traits<TIterator>::reference reference;
        typedef ::std::range_checked_iterator_tag checked_iterator_category;
        typedef TIterator inner_type;
        typedef TIterator checked_iterator_base_type;

        checked_iterator_base_type checked_iterator_base() const {
            return myContainer_ + current_;
        }

        void checked_iterator_assign_from_base(checked_iterator_base_type base) {
            current_ = base - myContainer_;
        }

        checked_array_iterator(): size_(0), current_(0) {
        }

        checked_array_iterator(TIterator container, size_t size, size_t index = 0) {
            {
                if (!(index <= size)) {
                    (void)(((("index <= size", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3430, 0, L"(\"index <= size\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3430, 0);
                }
            }
            myContainer_ = container;
            size_ = size;
            current_ = index;
        }

        TIterator base() const {
            return myContainer_ + current_;
        }

        size_t size() const {
            return size_;
        }

        bool operator==(const MyType& right) const {
            {
                if (!(myContainer_ == right.myContainer_)) {
                    (void)(((("myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3448, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3448, 0);
                }
            }
            return current_ == right.current_;
        }

        bool operator!=(const MyType& right) const {
            return !(*this == right);
        }

        bool operator<(const MyType& right) const {
            {
                if (!(myContainer_ == right.myContainer_)) {
                    (void)(((("myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3459, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3459, 0);
                }
            }
            return current_ < right.current_;
        }

        bool operator>(const MyType& right) const {
            return right < *this;
        }
        bool operator<=(const MyType& right) const {
            return !(right < *this);
        }

        bool operator>=(const MyType& right) const {
            return !(*this < right);
        }

        reference operator*() const {
            {
                if (!(current_ < size_)) {
                    (void)(((("current_ < size", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3480, 0, L"(\"current_ < size\", 0)")) || (__debugbreak(), 0));
                    ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3480, 0);
                }
            }
            return *(myContainer_ + current_);
        }

        pointer operator->() const {
            return (&** this);
            checked_array_iterator& operator++() {
                {
                    if (!(current_ < size_)) {
                        (void)(((("current_ < size", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3491, 0, L"(\"current_ < size\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3491, 0);
                    }
                }
                ++current_;
                return *this;
            }
            MyType operator++(int) {
                checked_array_iterator temp = *this;
                ++*this;
                return temp;
            }
            MyType& operator--() {
                {
                    if (!(current_ > 0)) {
                        (void)(((("current_ > 0", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3505, 0, L"(\"current_ > 0\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3505, 0);
                    };
                };
                --current_;
                return *this;
            }
            MyType operator--(int) {
                checked_array_iterator temp = *this;
                --*this;
                return temp;
            }
            MyType& operator+=(difference_type offset) {
                {
                    if (!(current_ + offset <= size_ && current_ + offset >= 0)) {
                        (void)(((("current_ + offset <= size && current_ + offset >= 0", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3521, 0, L"(\"current_ + offset <= size && current_ + offset >= 0\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3521, 0);
                    }
                }
                current_ += offset;
                return *this;
            }
            MyType operator+(difference_type offset) const {
                checked_array_iterator temp = *this;
                return (temp += offset);
            }
            MyType& operator-=(difference_type offset) {
                return (*this += -offset);
            }
            MyType operator-(difference_type offset) const {
                checked_array_iterator temp = *this;
                return (temp -= offset);
            }
            difference_type operator-(const checked_array_iterator & right) const {
                {
                    if (!(myContainer_ == right.myContainer_)) {
                        (void)(((("myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3545, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3545, 0);
                    }
                }
                return current_ - right.current_;
            }
            reference operator[](difference_type offset) const {
                {
                    if (!(current_ + offset < size_ && current_ + offset >= 0)) {
                        (void)(((("current_ + offset < size && current_ + offset >= 0", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xutility", 3551, 0, L"(\"current_ + offset < size && current_ + offset >= 0\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xutility", 3551, 0);
                    }
                }
                return *(myContainer_ + current_ + offset);
            }
protected:
            void xRange() const {
                throw ::std::out_of_range("invalid checked_array_iterator<T> subscript");
            }
            void xInvalidArg() const {
                throw ::std::invalid_argument("invalid checked_array_iterator<T> argument");
            }
            TIterator myContainer_;
            size_t current_;
            size_t size_;
        };

        template <class TIterator>
        checked_array_iterator<TIterator> make_checked_array_iterator(TIterator ptr, size_t size) {
            return checked_array_iterator<TIterator>(ptr, size);
        }

        template<class TInputInterator, class TOutputInterator> inline
        TOutputInterator __cdecl unchecked_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (::std::copy_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                    ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        __cdecl checked_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (::std::copy_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                    ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutElement, size_t size>
        inline
        TOutElement* __cdecl checked_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
            return checked_copy(::std::checked_base(first), ::std::checked_base(last), ::stdext::make_checked_array_iterator(destination, size)).base();
        }

        template<class TInputInterator, class TOutputInterator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        __cdecl checked_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (::std::copy_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                    ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2> inline
        TBidirectionIterator2 __cdecl unchecked_copy_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
            return (::std::copy_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                             ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
        __cdecl checked_copy_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
            return ::std::copy_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                            ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
        __cdecl checked_copy_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
            return ::std::copy_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                            ::std::iterator_random(first, destination), ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator> inline
        TOutputInterator __cdecl uncheckedmove(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (::std::move_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                    ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        __cdecl checkedmove(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return ::std::move_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                   ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, size_t size>
        inline
        TOutElement* __cdecl checkedmove(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
            return checkedmove(::std::checked_base(first), ::std::checked_base(last),
                               ::stdext::make_checked_array_iterator(destination, size)).base();
        }

        template<class TInputInterator, class TOutputInterator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        __cdecl checkedmove(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return ::std::move_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                   ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2> inline
        TBidirectionIterator2 __cdecl uncheckedmove_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
            return (::std::move_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                             ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
        __cdecl checkedmove_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination) {
            return ::std::move_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
                                            ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator2 >::result, TBidirectionIterator2 >::result
        __cdecl checkedmove_backward(TBidirectionIterator1 first, TBidirectionIterator1 last, TBidirectionIterator2 destination {
            return ::std::move_backward_opt(::std::checked_base(first), ::std::checked_base(last), destination,
            ::std::iterator_random(first, destination), ::std::move_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TOutputInterator, class TDifference, class T> inline
        void __cdecl unchecked_fill_n(TOutputInterator first, TDifference count, const T& value) {
            ::std::fill_n(first, count, value, ::std::iterator_category(first), ::std::range_checked_iterator_tag());
        }

        template<class TOutputInterator, class TDifference, class T>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        __cdecl checked_fill_n(TOutputInterator first, TDifference count, const T& value) {
            fill_n(first, count, value, ::std::iterator_category(first), ::std::range_checked_iterator_tag());
        }

        template<class TOutElement, class TDifference, class T, size_t size>
        inline
        void __cdecl checked_fill_n(TOutElement(&first)[size], TDifference count, const T& value) {
            checked_fill_n(::stdext::make_checked_array_iterator(first, size), count, value);
        }

        template<class TOutputInterator, class TDifference, class T>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        __cdecl checked_fill_n(TOutputInterator first, TDifference count, const T& value) {
            fill_n(first, count, value, ::std::iterator_category(first), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        ::std::pair<TInputIterator1, TInputIterator2>
        __cdecl unchecked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInElement2, size_t size>
        inline
        ::std::pair<TInputIterator1, TInElement2*>
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size]) {
            ::std::pair<TInputIterator1, ::stdext::checked_array_iterator<TInElement2*> > result =
                checked_mismatch(first1, last1, ::stdext::make_checked_array_iterator(first2, size));
            return (::std::pair<TInputIterator1, TInElement2*>(result.first, result.second.base()));
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        ::std::pair<TInputIterator1, TInputIterator2>
        __cdecl unchecked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInElement2, class TPredication, size_t size>
        inline
        ::std::pair<TInputIterator1, TInElement2*>
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size], TPredication predication) {
            ::std::pair<TInputIterator1, ::stdext::checked_array_iterator<TInElement2*> > result =
                checked_mismatch(first1, last1, ::stdext::make_checked_array_iterator(first2, size), predication);
            return (::std::pair<TInputIterator1, TInElement2*>(result.first, result.second.base()));
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, ::std::pair< TInputIterator1, TInputIterator2 > >::result
        __cdecl checked_mismatch(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            ::std::pair<typename ::std::checked_iterator_base_helper<TInputIterator1>::checked_iterator_base_type, TInputIterator2> result =
                ::std::mismatch(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
            ::std::checked_assign_from_base(first1, result.first);
            return (::std::pair<TInputIterator1, TInputIterator2>(first1, result.second));
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        bool __cdecl unchecked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
        __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInElement2, size_t size>
        inline
        bool __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size]) {
            return checked_equal(first1, last1,
                                 ::stdext::make_checked_array_iterator(first2, size));
        }

        template<class TInputIterator1, class TInputIterator2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
        __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        bool __cdecl unchecked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
        __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInElement2, class TPredication, size_t size>
        inline
        bool __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[size], TPredication predication) {
            return checked_equal(first1, last1,
                                 ::stdext::make_checked_array_iterator(first2, size), predication);
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, bool >::result
        __cdecl checked_equal(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TPredication predication) {
            return ::std::equal(::std::checked_base(first1), ::std::checked_base(last1), first2, predication,
                                ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }
    }

    namespace std {
        template<class TContainer>
        class back_insert_iterator : public OutputIterator {
        public:
            typedef TContainer container_type;
            typedef typename TContainer::reference reference;
            typedef range_checked_iterator_tag checked_iterator_category;
            explicit back_insert_iterator(TContainer& container) : container_(&container) {
            }

            back_insert_iterator<TContainer>& operator=(typename TContainer::const_reference value) {
                container_->push_back(value);
                return (*this);
            }

            back_insert_iterator<TContainer>& operator*() {
                return (*this);
            }

            back_insert_iterator<TContainer>& operator++() {
                return (*this);
            }

            back_insert_iterator<TContainer> operator++(int) {
                return (*this);
            }
        protected:
            TContainer* container_;
        };

        template<class TContainer> inline
        back_insert_iterator<TContainer> back_inserter(TContainer& container) {
            return (std::back_insert_iterator<TContainer>(container));
        }

        template<class TContainer>
        class front_insert_iterator : public OutputIterator {
        public:
            typedef TContainer container_type;
            typedef typename TContainer::reference reference;
            typedef range_checked_iterator_tag checked_iterator_category;
            explicit front_insert_iterator(TContainer& container)
                : container_(&container) {
            }

            front_insert_iterator<TContainer>& operator=(typename TContainer::const_reference value) {
                container_->push_front(value);
                return (*this);
            }

            front_insert_iterator<TContainer>& operator*() {
                return (*this);
            }

            front_insert_iterator<TContainer>& operator++() {
                return (*this);
            }

            front_insert_iterator<TContainer> operator++(int) {
                return (*this);
            }
        protected:
            TContainer* container_;
        };

        template<class TContainer> inline
        front_insert_iterator<TContainer> front_inserter(TContainer& container) {
            return (std::front_insert_iterator<TContainer>(container));
        }

        template<class TContainer>
        class insert_iterator : public OutputIterator {
        public:
            typedef TContainer container_type;
            typedef typename TContainer::reference reference;
            typedef range_checked_iterator_tag checked_iterator_category;
            insert_iterator(TContainer& container, typename TContainer::iterator where)
                : container_(&container), iterator_(where) {
            }

            insert_iterator<TContainer>& operator=(typename TContainer::const_reference value) {
                iterator_ = container->insert(iterator_, value);
                ++iterator_;
                return (*this);
            }

            insert_iterator<TContainer>& operator*() {
                return (*this);
            }

            insert_iterator<TContainer>& operator++() {
                return (*this);
            }

            insert_iterator<TContainer>& operator++(int) {
                return (*this);
            }
        protected:
            TContainer* container_;
            typename TContainer::iterator iterator_;
        };

        template<class TContainer, class TIterator> inline
        insert_iterator<TContainer> inserter(TContainer& container, TIterator where) {
            return (std::insert_iterator<TContainer>(container, where));
        }

        template < class T, class TElement = char, class TTraits = char_traits<TElement>, class TDifference = ptrdiff_t >
        class istream_iterator
                : public iterator<input_iterator_tag, T, TDifference, const T*, const T&> {
        public:
            typedef istream_iterator<T, TElement, TTraits, TDifference> MyType;
        public:
            typedef TElement char_type;
            typedef TTraits traits_type;
            typedef basic_istream<TElement, TTraits> istream_type;
            typedef range_checked_iterator_tag checked_iterator_category;
            istream_iterator() : myIStream_(0) {
            }

            istream_iterator(istream_type& istream) : myIStream_(&istream) {
                getValue_();
            }

            const T& operator*() const {
                if (myIStream_ == 0) {
                    debug_message(L"istream_iterator is not dereferencable", L"..\\ce\\include\\iterator", 202);
                    {
                        (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 203, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 203, 0);
                    }
                }

                return (myValue_);
            }

            const T* operator->() const {
                return (&** this);
            }

            MyType& operator++() {
                if (myIStream_ == 0) {
                    debug_message(L"istream_iterator is not incrementable", L"..\\ce\\include\\iterator", 223);
                    {
                        (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 224, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 224, 0);
                    }
                }

                getValue_();
                return (*this);
            }

            MyType operator++(int) {
                MyType temp = *this;
                ++*this;
                return (temp);
            }

            bool equal(const MyType& right) const {
                return (myIStream_ == right.myIStream_);
            }
        protected:
            void getValue_() {
                if (myIStream_ != 0 && !(*myIStream_ >> myValue_))
                    myIStream_ = 0;
            }

            static void xRange() {
                throw out_of_range("invalid istream_iterator");
            }

            istream_type* myIStream_;
            T myValue_;
        };

        template<class T, class TElement, class TTraits, class TDifference> inline
        bool operator==(
            const istream_iterator<T, TElement, TTraits, TDifference>& left,
            const istream_iterator<T, TElement, TTraits, TDifference>& right) {
            return (left.equal(right));
        }

        template<class T, class TElement, class TTraits, class TDifference> inline
        bool operator!=(
            const istream_iterator<T, TElement, TTraits, TDifference>& left,
            const istream_iterator<T, TElement, TTraits, TDifference>& right) {
            return (!(left == right));
        }

        template < class T, class TElement = char, class TTraits = char_traits<TElement> >
        class ostream_iterator : public OutputIterator {
        public:
            typedef TElement char_type;
            typedef TTraits traits_type;
            typedef basic_ostream<TElement, TTraits> ostream_type;
            typedef range_checked_iterator_tag checked_iterator_category;
            ostream_iterator(ostream_type& ostream, const TElement* delim = 0)
                : myOStream_(&ostream), myDelim_(delim) {
            }

            ostream_iterator<T, TElement, TTraits>& operator=(const T& value) {
                *myOStream_ << value;

                if (myDelim_ != 0)
                    *myOStream_ << myDelim_;

                if (!*myOStream_) {
                    debug_message(L"ostream_iterator is not dereferencable", L"..\\ce\\include\\iterator", 316);
                    {
                        (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 317, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 317, 0);
                    }
                }

                return (*this);
            }

            ostream_iterator<T, TElement, TTraits>& operator*() {
                return (*this);
            }

            ostream_iterator<T, TElement, TTraits>& operator++() {
                return (*this);
            }

            ostream_iterator<T, TElement, TTraits> operator++(int) {
                return (*this);
            }
        protected:
            static void xRange() {
                throw out_of_range("invalid ostream_iterator");
            }

            const TElement* myDelim_;
            ostream_type* myOStream_;
        };
    }

    namespace stdext {
        template < class container, class TIterator = typename container::iterator >
        class checked_iterator
            : public ::std::iterator <
            typename ::std::iterator_traits<TIterator>::iterator_category,
            typename ::std::iterator_traits<TIterator>::value_type,
            typename ::std::iterator_traits<TIterator>::difference_type,
            typename ::std::iterator_traits<TIterator>::pointer,
                typename ::std::iterator_traits<TIterator>::reference > {
            friend class checked_iterator;
        public:
            typedef checked_iterator<container, TIterator> MyType;
            typedef typename ::std::iterator_traits<TIterator>::difference_type difference_type;
            typedef typename ::std::iterator_traits<TIterator>::pointer pointer;
            typedef typename ::std::iterator_traits<TIterator>::reference reference;
            typedef ::std::range_checked_iterator_tag checked_iterator_category;
            typedef TIterator inner_type;
            typedef TIterator checked_iterator_base_type;

            checked_iterator_base_type checked_iterator_base() const {
                return current_;
            }

            void checked_iterator_assign_from_base(checked_iterator_base_type base) {
                current_ = base;
            }

            checked_iterator() : myContainer_(0) {
            }

            checked_iterator(container& c, TIterator ptr) : myContainer_(&c), current_(ptr) {
            }

            checked_iterator(const MyType& right) : myContainer_(right.myContainer_), current_(right.current_) {
            }

            template <class TIterator2>
            checked_iterator(const checked_iterator<container, TIterator2> &right)
                : myContainer_(right.myContainer_), current_(right.current_) {
            }

            TIterator base() const {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 412, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 412, 0);
                    }
                }
                return current_;
            }

            template <class TIterator2>
            bool operator==(const checked_iterator<container, TIterator2>& right) const {
                {
                    if (!(myContainer_ == right.myContainer_)) {
                        (void)(((("myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 419, 0, L"(\"myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 419, 0);
                    }
                }
                return current_ == right.current_;
            }

            template <class TIterator2>
            bool operator!=(const checked_iterator<container, TIterator2>& right) const {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 426, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 426, 0);
                    }
                }
                return !(*this == right);
            }

            template <class TIterator2>
            bool operator<(const checked_iterator<container, TIterator2>& right) const {
                {
                    if (!(myContainer_ != 0 && myContainer_ == right.myContainer_)) {
                        (void)(((("myContainer_ != NULL && myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 433, 0, L"(\"myContainer_ != NULL && myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 433, 0);
                    }
                }
                return current_ < right.current_;
            }

            template <class TIterator2>
            bool operator>(const checked_iterator<container, TIterator2>& right) const {
                return right < *this;
            }

            template <class TIterator2>
            bool operator<=(const checked_iterator<container, TIterator2>& right) const {
                return !(right < *this);
            }

            template <class TIterator2>
            bool operator>=(const checked_iterator<container, TIterator2>& right) const {
                return !(*this < right);
            }

            reference operator*() const {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 457, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 457, 0);
                    }
                }
                {
                    if (!(current_ != myContainer_->end())) {
                        (void)(((("current_ != myContainer_->end()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 458, 0, L"(\"current_ != myContainer_->end()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 458, 0);
                    }
                }
                return *current_;
            }

            pointer operator->() const {
                return (&** this);
            }

            MyType& operator++() {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 469, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 469, 0);
                    }
                }
                {
                    if (!(current_ != myContainer_->end())) {
                        (void)(((("current_ != myContainer_->end()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 470, 0, L"(\"current_ != myContainer_->end()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 470, 0);
                    }
                }
                ++current_;
                return *this;
            }

            MyType operator++(int) {
                MyType temp = *this;
                ++*this;
                return temp;
            }

            MyType& operator--() {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 484, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 484, 0);
                    }
                }
                {
                    if (!(current_ != myContainer_->begin())) {
                        (void)(((("current_ != myContainer_->begin()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 485, 0, L"(\"current_ != myContainer_->begin()\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 485, 0);
                    }
                }
                --current_;
                return *this;
            }

            MyType operator--(int) {
                MyType temp = *this;
                --*this;
                return temp;
            }

            MyType& operator+=(difference_type offset) {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 501, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 501, 0);
                    }
                }
                {
                    if (!((myContainer_->end() - current_) >= offset && (myContainer_->begin() - current_) <= offset)) {
                        (void)(((("(myContainer_->end() - current_) >= offset && (myContainer_->begin() - current_) <= offset", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 502, 0, L"(\"(myContainer_->end() - current_) >= offset && (myContainer_->begin() - current_) <= offset\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 502, 0);
                    }
                }
                current_ += offset;
                return *this;
            }

            MyType operator+(difference_type offset) const {
                MyType temp = *this;
                return (temp += offset);
            }

            MyType& operator-=(difference_type offset) {
                return (*this += -offset);
            }

            MyType operator-(difference_type offset) const {
                MyType temp = *this;
                return (temp -= offset);
            }

            difference_type operator-(const MyType& right) const {
                {
                    if (!(myContainer_ != 0 && myContainer_ == right.myContainer_)) {
                        (void)(((("myContainer_ != NULL && myContainer_ == right.myContainer_", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 526, 0, L"(\"myContainer_ != NULL && myContainer_ == right.myContainer_\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 526, 0);
                    }
                }
                return current_ - right.current_;
            }

            reference operator[](difference_type offset) const {
                {
                    if (!(myContainer_ != 0)) {
                        (void)(((("myContainer_ != NULL", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 532, 0, L"(\"myContainer_ != NULL\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 532, 0);
                    }
                }
                {
                    if (!((myContainer_->end() - current_) > offset && (myContainer_->begin() - current_) <= offset)) {
                        (void)(((("(myContainer_->end() - current_) > offset && (myContainer_->begin() - current_) <= offset", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\iterator", 533, 0, L"(\"(myContainer_->end() - current_) > offset && (myContainer_->begin() - current_) <= offset\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\iterator", 533, 0);
                    }
                }
                return current_[offset];
            }
        protected:
            void xRange() const {
                throw ::std::out_of_range("invalid checked_iterator<T> subscript");
            }

            void xInvalidArg() const {
                throw ::std::invalid_argument("invalid checked_iterator<T> argument");
            }

            container* myContainer_;
            TIterator current_;
        };
    }

    namespace std {
        using ::size_t;
        using ::div_t;
        using ::ldiv_t;
        using ::abs;
        using ::atexit;
        using ::atof;
        using ::atoi;
        using ::atol;
        using ::calloc;
        using ::div;
        using ::exit;
        using ::free;
        using ::labs;
        using ::ldiv;
        using ::malloc;
        using ::mbstowcs;
        using ::qsort;
        using ::rand;
        using ::realloc;
        using ::srand;
        using ::strtod;
        using ::strtol;
        using ::strtoul;
        using ::wcstombs;
    }

    namespace std {
        typedef void (__cdecl* terminate_function)();
        typedef void (__cdecl* unexpected_function)();
        typedef void (__cdecl* terminate_handler)();
        typedef void (__cdecl* unexpected_handler)();
        terminate_function __cdecl set_terminate(terminate_function);
        unexpected_function __cdecl set_unexpected(unexpected_function);
        void __cdecl terminate(void);
        void __cdecl unexpected(void);
    }

    namespace std {
        class  exception {
        public:
            exception();
            exception(const char*) throw();
            exception(const char* const& what, int) {
                what_ = what;
                doFree_ = 0;
            }
            exception(const exception&);
            exception&  operator=(const exception&);
            virtual  ~exception();
            virtual const char*   what() const;
        private:
            const char* what_;
            int doFree_;
        };
        using std::set_terminate;
        using std::terminate_handler;
        using std::terminate;
        using std::set_unexpected;
        using std::unexpected_handler;
        using std::unexpected;
        typedef void (__cdecl* _Prhand)(const exception&);
        bool __cdecl uncaught_exception();
    }

    namespace std {
        class bad_exception : public exception {
        public:
            bad_exception(const char* message = "bad exception")
            throw()
                : exception(message) {
            }

            virtual  ~bad_exception() throw() {
            }
        };

        static const char* _bad_alloc_Message = "bad allocation";
        class bad_alloc : public exception {
        public:
            bad_alloc(const char* message) throw()
                : exception(message) {
            }

            bad_alloc() throw()
                : exception(_bad_alloc_Message, 1) {
            }

            virtual  ~bad_alloc() throw() {
            }
        };
    }

    namespace std {
        struct nothrow_t {};
        extern const nothrow_t nothrow;
    }

    namespace std {
        typedef void (__cdecl* new_handler)();
        new_handler __cdecl set_new_handler(new_handler _NewHandler) throw();
        //using ::new_handler;
        //using ::set_new_handler;
    }

    void* __cdecl operator new(size_t size, const std::nothrow_t&) throw();
    void* __cdecl operator new[](size_t size, const std::nothrow_t&) throw();
    void __cdecl operator delete(void*, const std::nothrow_t&) throw();
    void __cdecl operator delete[](void*, const std::nothrow_t&) throw();
    inline void* __cdecl operator new(size_t, void* where) {
        return (where);
    }

    inline void __cdecl operator delete(void*, void*) {
        return;
    }

    int __cdecl _query_new_mode(void);
    int __cdecl _set_new_mode(int);
    typedef int (__cdecl* _PNH)(size_t);
    _PNH __cdecl _query_new_handler(void);
    _PNH __cdecl _set_new_handler(_PNH _NewHandler);

    void __cdecl operator delete(void*) throw();
#pragma warning (suppress: 4985)

[returnvalue:SA_Post(Null=SA_No, WritableBytes="size")]
    void* __cdecl operator new(size_t size) throw(...);
    inline void* __cdecl operator new[](size_t, void* where) throw() {
        return (where);
    }

    inline void __cdecl operator delete[](void*, void*) throw() {
    }

    void __cdecl operator delete[](void*) throw();
[returnvalue:SA_Post(Null=SA_No, WritableBytes="size")]
    void* __cdecl operator new[](size_t size) throw(...);

    namespace std {
        template<class T> inline
        T*  allocate(size_t count, T*) {
            if (count <= 0)
                count = 0;
            else if (((size_t)(-1) / count) < sizeof(T))
                throw std::bad_alloc(0);

            return ((T*)::operator new(count * sizeof(T)));
        }

        template<class T1, class T2> inline
        void construct(T1*  ptr, const T2& value) {
            void*  vptr = ptr;
            ::new(vptr) T1(value);
        }

        template<class T> inline
        void destroy(T*  ptr) {
            (ptr)->~T();
        }

        template<> inline
        void destroy(char*) {
        }

        template<> inline
        void destroy(wchar_t*) {
        }

        template<class T>
        struct allocator_base {
            typedef T value_type;
        };

        template<class T>
        struct allocator_base<const T> {
            typedef T value_type;
        };

        template<class T>
        class allocator
                : public allocator_base<T> {
        public:
            typedef allocator_base<T> MyBaseType;
            typedef typename MyBaseType::value_type value_type;
            typedef value_type*  pointer;
            typedef value_type& reference;
            typedef const value_type*  const_pointer;
            typedef const value_type& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            template<class TOther>
            struct rebind {
                typedef allocator<TOther> other;
            };

            pointer address(reference value) const {
                return (&value);
            }

            const_pointer address(const_reference value) const {
                return (&value);
            }

            allocator() throw() {
            }

            allocator(const allocator<T>&) throw() {
            }

            template<class TOther>
            allocator(const allocator<TOther>&) throw() {
            }

            template<class TOther>
            allocator<T>& operator=(const allocator<TOther>&) {
                return (*this);
            }

            void deallocate(pointer ptr, size_type) {
                ::operator delete(ptr);
            }

            pointer allocate(size_type count) {
                return (allocate(count, (pointer)0));
            }

            pointer allocate(size_type count, const void*) {
                return (allocate(count));
            }

            void construct(pointer ptr, const T& value) {
                construct(ptr, value);
            }

            void destroy(pointer ptr) {
                destroy(ptr);
            }

            size_t maxSize() const throw() {
                size_t count = (size_t)(-1) / sizeof(T);
                return (0 < count ? count : 1);
            }
        };

        template<class T, class TOther> inline
        bool operator==(const allocator<T>&, const allocator<TOther>&) throw() {
            return (true);
        }

        template<class T, class TOther> inline
        bool operator!=(const allocator<T>&, const allocator<TOther>&) throw() {
            return (false);
        }

        template<> class  allocator<void> {
        public:
            typedef void T;
            typedef T*  pointer;
            typedef const T*  const_pointer;
            typedef T value_type;

            template<class TOther>
            struct rebind {
                typedef allocator<TOther> other;
            };

            allocator() throw() {
            }

            allocator(const allocator<T>&) throw() {
            }

            template<class TOther>
            allocator(const allocator<TOther>&) throw() {
            }

            template<class TOther>
            allocator<T>& operator=(const allocator<TOther>&) {
                return (*this);
            }
        };

        template<class Allocator> inline
        void destroy_range(typename Allocator::pointer first,
                           typename Allocator::pointer last, Allocator& allocator) {
            destroy_range(first, last, allocator, ptr_category(first, last));
        }

        template<class Allocator> inline
        void destroy_range(typename Allocator::pointer first,
                           typename Allocator::pointer last, Allocator& allocator,
                           nonscalar_ptr_iterator_tag) {
            for (; first != last; ++first)
                allocator.destroy(first);
        }

        template<class Allocator> inline
        void destroy_range(typename Allocator::pointer first,
                           typename Allocator::pointer last, Allocator& allocator,
                           scalar_ptr_iterator_tag) {
        }
    }

    namespace std {
        template<class T> inline
        pair<T*, ptrdiff_t>
        get_temporary_buffer(ptrdiff_t count) {
            T*  pBuffer;

            if (count <= 0)
                count = 0;
            else if (((size_t)(-1) / count) < sizeof(T))
                throw std::bad_alloc(0);

            for (pBuffer = 0; 0 < count; count /= 2)
                if ((pBuffer = (T*)operator new(
                                   (size_t)count * sizeof(T), nothrow)) != 0)
                    break;

            return (pair<T*, ptrdiff_t>(pBuffer, count));
        }

        template<class T> inline
        void return_temporary_buffer(T* pBuffer) {
            operator delete(pBuffer);
        }

        template<class TInputInterator, class TForwardIterator> inline
        TForwardIterator uninit_copy(TInputInterator first, TInputInterator last, TForwardIterator destination,
                                     nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 48);
            debug_pointer(destination, L"..\\ce\\include\\memory", 49);
            TForwardIterator next = destination;

            try {
                for (; first != last; ++destination, ++first)
                    construct(&*destination, *first);
            } catch (...) {
                for (; next != destination; ++next)
                    destroy(&*next);

                throw;
            }

            return (destination);
        }

        template<class T1, class T2> inline
        T2 uninit_copy(T1 first, T1 last, T2 destination,
                       scalar_ptr_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 68);
            debug_pointer(destination, L"..\\ce\\include\\memory", 69);
            size_t count = (size_t)(last - first);
            T2 result = destination + count;

            if (count > 0)
                ::memmove_s((&*destination), (count * sizeof(*first)), (&*first), (count * sizeof(*first)));

            return result;
        }

        template<class TInputInterator, class TForwardIterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination) {
            return (uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, size_t size>
        inline
        TForwardElement* uninitialized_copy(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size]) {
            return (uninitialized_copy(first, last,
                                       ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputInterator, class TForwardIterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination) {
            return (uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        TForwardIterator uninit_copy(TInputInterator first, TInputInterator last, TForwardIterator destination,
                                     Allocator& allocator, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 123);
            debug_pointer(destination, L"..\\ce\\include\\memory", 124);
            TForwardIterator next = destination;

            try {
                for (; first != last; ++destination, ++first)
                    allocator.construct(destination, *first);
            } catch (...) {
                for (; next != destination; ++next)
                    allocator.destroy(next);

                throw;
            }

            return (destination);
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        TForwardIterator uninit_copy(TInputInterator first, TInputInterator last, TForwardIterator destination,
                                     Allocator&, scalar_ptr_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 144);
            debug_pointer(destination, L"..\\ce\\include\\memory", 145);
            size_t count = (size_t)(last - first);
            TForwardIterator result = destination + count;

            if (count > 0)
                ::memmove_s((&*destination), (count * sizeof(*first)), (&*first), (count * sizeof(*first)));

            return result;
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination,
                           Allocator& allocator) {
            return (uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, class Allocator, size_t size>
        inline
        TForwardElement* uninitialized_copy(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size],
                                            Allocator& allocator) {
            return (uninitialized_copy(first, last,
                                       ::stdext::make_checked_array_iterator(destination, size), allocator).base());
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination,
                           Allocator& allocator) {
            return (uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator, class TMoveCategory>
        inline
        TForwardIterator uninitmove(TInputInterator first, TInputInterator last, TForwardIterator destination,
                                    Allocator& allocator, TMoveCategory, range_checked_iterator_tag) {
            return ::stdext::unchecked_uninitialized_copy(first, last, destination, allocator);
        }

        template<class TInputInterator, class TForwardIterator, class Allocator>
        inline
        TForwardIterator uninitmove(TInputInterator first, TInputInterator last, TForwardIterator destination,
                                    Allocator& allocator, swapmove_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 213);
            debug_pointer(destination, L"..\\ce\\include\\memory", 214);
            TForwardIterator next = destination;
            typename Allocator::value_type value;

            try {
                for (; first != last; ++destination, ++first) {
                    allocator.construct(destination, value);
                    ::std::swap(*destination, *first);
                }
            } catch (...) {
                for (; next != destination; ++next)
                    allocator.destroy(next);

                throw;
            }

            return (destination);
        }

        template<class TInputInterator, class TForwardIterator, class TAllocator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitializedmove(TInputInterator first, TInputInterator last, TForwardIterator destination,
                          TAllocator& allocator) {
            return (uninitmove(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                               move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, class Allocator, size_t size>
        inline
        TForwardElement* uninitializedmove(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size],
                                           Allocator& allocator) {
            return (uninitializedmove(first, last,
                                      ::stdext::make_checked_array_iterator(destination, size), allocator).base());
        }

        template<class TInputInterator, class TForwardIterator, class Allocator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        uninitializedmove(TInputInterator first, TInputInterator last, TForwardIterator destination,
                          Allocator& allocator) {
            return (uninitmove(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                               move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TForwardIterator, class TValue> inline
        void uninit_fill(TForwardIterator first, TForwardIterator last, const TValue& value, nonscalar_ptr_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\memory", 282);
            TForwardIterator next = first;

            try {
                for (; first != last; ++first)
                    construct(&*first, value);
            } catch (...) {
                for (; next != first; ++next)
                    destroy(&*next);

                throw;
            }
        }

        template<class T, class TValue> inline
        void uninit_fill(T* first, T* last, const TValue& value,
                         scalar_ptr_iterator_tag) {
            std::fill(first, last, value);
        }

        template<class TForwardIterator, class TValue> inline
        void uninitialized_fill(TForwardIterator first, TForwardIterator last, const TValue& value) {
            uninit_fill(::std::checked_base(first), ::std::checked_base(last), value, ptr_category(first, first));
        }
        template<class TForwardIterator, class TDifference, class TValue> inline
        void uninit_fill_n(TForwardIterator first, TDifference count, const TValue& value,
                           nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
            TForwardIterator next = first;

            try {
                for (; 0 < count; --count, ++first)
                    construct(&*first, value);
            } catch (...) {
                for (; next != first; ++next)
                    destroy(&*next);

                throw;
            }
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        void uninit_fill_n(TForwardIterator first, TDifference count, const TValue& value,
                           scalar_ptr_iterator_tag, range_checked_iterator_tag) {
            ::stdext::unchecked_fill_n(&*first, count, value);
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value) {
            uninit_fill_n(first, count, value, ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardElement, class TDifference, class TValue, size_t size>
        inline
        void uninitialized_fill_n(TForwardElement(&first)[size], TDifference count, const TValue& value) {
            uninitialized_fill_n(::stdext::make_checked_array_iterator(first, size), count, value);
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value) {
            uninit_fill_n(first, count, value, ptr_category(first, first), ::std::range_checked_iterator_tag());
        }
        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        void uninit_fill_n(TForwardIterator first, TDifference count,
                           const TValue& value, Allocator& allocator, nonscalar_ptr_iterator_tag, range_checked_iterator_tag) {
            TForwardIterator next = first;

            try {
                for (; 0 < count; --count, ++first)
                    allocator.construct(first, value);
            } catch (...) {
                for (; next != first; ++next)
                    allocator.destroy(next);

                throw;
            }
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        void uninit_fill_n(TForwardIterator first, TDifference count,
                           const TValue& value, Allocator&, scalar_ptr_iterator_tag, range_checked_iterator_tag) {
            ::stdext::unchecked_fill_n(first, count, value);
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        uninitialized_fill_n(TForwardIterator first, TDifference count,
                             const TValue& value, Allocator& allocator) {
            uninit_fill_n(first, count, value, allocator,
                          ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardElement, class TDifference, class TValue, class Allocator, size_t size>
        inline
        void uninitialized_fill_n(TForwardElement(&first)[size], TDifference count,
                                  const TValue& value, Allocator& allocator) {
            uninitialized_fill_n(::stdext::make_checked_array_iterator(first, size), count, value, allocator);
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, void >::result uninitialized_fill_n(TForwardIterator first, TDifference count,
                const TValue& value, Allocator& allocator) {
            uninit_fill_n(first, count, value, allocator,
                          ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class T>
        class raw_storage_iterator : public OutputIterator {
        public:
            typedef TForwardIterator iterator_type;
            typedef TForwardIterator iter_type;
            typedef T element_type;
            explicit raw_storage_iterator(TForwardIterator first)
                : next_(first) {
            }

            raw_storage_iterator<TForwardIterator, T>& operator*() {
                return (*this);
            }

            raw_storage_iterator<TForwardIterator, T>& operator=(const T& value) {
                construct(&*next_, value);
                return (*this);
            }

            raw_storage_iterator<TForwardIterator, T>& operator++() {
                ++next_;
                return (*this);
            }

            raw_storage_iterator<TForwardIterator, T> operator++(int) {
                raw_storage_iterator<TForwardIterator, T> ans = *this;
                ++next_;
                return (ans);
            }
        private:
            TForwardIterator next +;
        };

        template<class T>
        class TempIterator : public OutputIterator {
        public:
            typedef T* PtrType;
            typedef range_checked_iterator_tag checked_iterator_category;
            TempIterator(ptrdiff_t count = 0) {
                buffer_.Begin = 0;
                buffer_.Current = 0;
                buffer_.HiWater = 0;
                buffer_.Size = count;
                pBuffer_ = &buffer_;
            }

            TempIterator(const TempIterator<T>& right) {
                buffer_.Begin = 0;
                buffer_.Current = 0;
                buffer_.HiWater = 0;
                buffer_.Size = 0;
                *this = right;
            }

            ~TempIterator() {
                if (buffer_.Begin != 0) {
                    for (PtrType next = buffer_.Begin;
                            next != buffer_.HiWater; ++next)
                        destroy(&*next);

                    std::return_temporary_buffer(buffer_.Begin);
                }
            }

            TempIterator<T>& operator=(const TempIterator<T>& right) {
                pBuffer_ = right.pBuffer_;
                return (*this);
            }

            TempIterator<T>& operator=(const T& value) {
                if (pBuffer_->Current < pBuffer_->HiWater)
                    *pBuffer_->Current++ = value;
                else {
                    {
                        if (!((pBuffer_->Current - pBuffer_->Begin) < pBuffer_->Size)) {
                            (void)(((("(pBuffer_->Current - pBuffer_->Begin) < pBuffer_->Size", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\memory", 563, 0, L"(\"(pBuffer_->Current - pBuffer_->Begin) < pBuffer_->Size\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\memory", 563, 0);
                        }
                    }
                    PtrType ptr = &*pBuffer_->Current;
                    construct(ptr, value);
                    pBuffer_->HiWater = ++pBuffer_->Current;
                }

                return (*this);
            }

            TempIterator<T>& operator*() {
                return (*this);
            }

            TempIterator<T>& operator++() {
                return (*this);
            }

            TempIterator<T>& operator++(int) {
                return (*this);
            }

            TempIterator<T>& init() {
                pBuffer_->Current = pBuffer_->Begin;
                return (*this);
            }

            PtrType first() const {
                return (pBuffer_->Begin);
            }

            PtrType last() const {
                return (pBuffer_->Current);
            }

            ptrdiff_t maxLength() {
                if (pBuffer_->Begin == 0 && 0 < pBuffer_->Size) {
                    pair<PtrType, ptrdiff_t> pair = std::get_temporary_buffer<T>(pBuffer_->Size);
                    pBuffer_->Begin = pair.first;
                    pBuffer_->Current = pair.first;
                    pBuffer_->HiWater = pair.first;
                    pBuffer_->Size = pair.second;
                }

                return (pBuffer_->Size);
            }

            static void xInvalidArg() {
                throw invalid_argument("invalid TempIterator<T> argument");
            }
        private:
            struct BufPar {
                PtrType Begin;
                PtrType Current;
                PtrType HiWater;
                ptrdiff_t Size;
            };
            BufPar buffer_;
            BufPar* pBuffer_;
        };

        template<class T>
        class auto_ptr;
        template<class T>
        struct auto_ptr_ref {
            explicit auto_ptr_ref(T* right) : ref(right) {
            }
            T* ref;
        };

        template<class T>
        class auto_ptr {
        public:
            typedef T element_type;
            explicit auto_ptr(T* ptr = 0) throw()
                : myPtr_(ptr) {
            }

            auto_ptr(auto_ptr<T>& right) throw()
                : myPtr_(right.release()) {
            }

            auto_ptr(auto_ptr_ref<T> right) throw() {
                T* ptr = right.ref;
                right.ref = 0;
                myPtr_ = ptr;
            }

            template<class TOther>
            operator auto_ptr<TOther>() throw() {
                return (auto_ptr<TOther>(*this));
            }

            template<class TOther>
            operator auto_ptr_ref<TOther>() throw() {
                TOther* convertPtr = myPtr_;
                auto_ptr_ref<TOther> ans(convertPtr);
                myPtr_ = 0;
                return (ans);
            }

            template<class TOther>
            auto_ptr<T>& operator=(auto_ptr<TOther>& right) throw() {
                reset(right.release());
                return (*this);
            }

            template<class TOther>
            auto_ptr(auto_ptr<TOther>& right) throw()
                : myPtr_(right.release()) {
            }

            auto_ptr<T>& operator=(auto_ptr<T>& right) throw() {
                reset(right.release());
                return (*this);
            }

            auto_ptr<T>& operator=(auto_ptr_ref<T> right) throw() {
                T* ptr = right.ref;
                right.ref = 0;
                reset(ptr);
                return (*this);
            }

            ~auto_ptr() {
                if (myPtr_ != 0)
                    delete myPtr_;
            }

            T& operator*() const throw() {
                if (myPtr_ == 0)
                    debug_message(L"auto_ptr not dereferencable", L"..\\ce\\include\\memory", 726);

                ;
                return (*get());
            }

            T* operator->() const throw() {
                if (myPtr_ == 0)
                    debug_message(L"auto_ptr not dereferencable", L"..\\ce\\include\\memory", 739);

                return (get());
            }

            T* get() const throw() {
                return (myPtr_);
            }

            T* release() throw() {
                T* temp = myPtr_;
                myPtr_ = 0;
                return (temp);
            }

            void reset(T* ptr = 0) {
                if (ptr != myPtr_ && myPtr_ != 0)
                    delete myPtr_;

                myPtr_ = ptr;
            }
        private:
            T* myPtr_;
        };
    }

    namespace stdext {
        template<class TInputInterator, class TForwardIterator> inline
        TForwardIterator unchecked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, size_t size>
        inline
        TForwardElement* checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size]) {
            return (checked_uninitialized_copy(first, last,
                                               ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputInterator, class TForwardIterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        TForwardIterator unchecked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, class Allocator, size_t size>
        inline
        TForwardElement* checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size], Allocator& allocator) {
            return (checked_uninitialized_copy(first, last,
                                               ::stdext::make_checked_array_iterator(destination, size), allocator).base());
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitialized_copy(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninit_copy(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                       ::std::ptr_category(first, destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator> inline
        TForwardIterator unchecked_uninitializedmove(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninitmove(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                      ::std::move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardIterator, class Allocator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitializedmove(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninitmove(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                      ::std::move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TForwardElement, class Allocator, size_t size>
        inline
        TForwardElement* checked_uninitializedmove(TInputInterator first, TInputInterator last, TForwardElement(&destination)[size], Allocator& allocator) {
            return (checked_uninitializedmove(first, last,
                                              ::stdext::make_checked_array_iterator(destination, size), allocator).base());
        }

        template<class TInputInterator, class TForwardIterator, class Allocator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, TForwardIterator >::result
        checked_uninitializedmove(TInputInterator first, TInputInterator last, TForwardIterator destination, Allocator& allocator) {
            return (::std::uninitmove(::std::checked_base(first), ::std::checked_base(last), destination, allocator,
                                      ::std::move_category(destination), ::std::range_checked_iterator_tag()));
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        void unchecked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value) {
            ::std::uninit_fill_n(first, count, value, ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, void >::result checked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value) {
            ::std::uninit_fill_n(first, count, value, ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }
        template<class TForwardElement, class TDifference, class TValue, size_t size>
        inline
        void checked_uninitialized_fill_n(TForwardElement(&first)[size], TDifference count, const TValue& value) {
            checked_uninitialized_fill_n(::stdext::make_checked_array_iterator(first, size), count, value);
        }

        template<class TForwardIterator, class TDifference, class TValue> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        checked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value) {
            ::std::uninit_fill_n(first, count, value, ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        void unchecked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value, Allocator& allocator) {
            ::std::uninit_fill_n(first, count, value, allocator,
                                 ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        checked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value, Allocator& allocator) {
            ::std::uninit_fill_n(first, count, value, allocator,
                                 ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }

        template<class TForwardElement, class TDifference, class TValue, class Allocator, size_t size>
        inline
        void checked_uninitialized_fill_n(TForwardElement(&first)[size], TDifference count,
                                          const TValue& value, Allocator& allocator) {
            checked_uninitialized_fill_n(::stdext::make_checked_array_iterator(first, size), count, value, allocator);
        }

        template<class TForwardIterator, class TDifference, class TValue, class Allocator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator >::result, void >::result
        checked_uninitialized_fill_n(TForwardIterator first, TDifference count, const TValue& value, Allocator& allocator) {
            ::std::uninit_fill_n(first, count, value, allocator,
                                 ::std::ptr_category(first, first), ::std::range_checked_iterator_tag());
        }
    }

    namespace std {
        struct type_info_node {
            void* memPtr;
            type_info_node* next;
        };

        extern type_info_node type_info_root_node;

        class type_info {
        public:
            virtual ~type_info();

            int __thiscall operator==(const type_info& rhs) const;
            int __thiscall operator!=(const type_info& rhs) const;
            int __thiscall before(const type_info& rhs) const;
            const char* name() const;
            const char* raw_name() const;
        private:
            void* data_;
            char d_name_[1];
            type_info(const type_info& rhs);
            type_info&  operator=(const type_info& rhs);
        };

        //using ::type_info;
    }

    namespace std {
        class  bad_cast : public exception {
        public:
            bad_cast(const char* message = "bad cast");
            bad_cast(const bad_cast&);
            virtual  ~bad_cast();
        };
        class  bad_typeid : public exception {
        public:
            bad_typeid(const char* message = "bad typeid");
            bad_typeid(const bad_typeid&);
            virtual  ~bad_typeid();
        };
        class  non_rtti_object : public bad_typeid {
        public:
            non_rtti_object(const char* message);
            non_rtti_object(const non_rtti_object&);
            virtual  ~non_rtti_object();
        };
    }

    namespace std {
#pragma warning(disable:4251)
        template < class TElement,
                 class TTraits = char_traits<TElement>,
                 class TAllocator = allocator<TElement> >
        class basic_string;
        template<class TElement, class TTraits, class Allocator>
        class string_const_iterator
            : public random_iterator_base < TElement, typename Allocator::difference_type,
                  typename Allocator::const_pointer, typename Allocator::const_reference, iterator_base_secure > {
        public:
            typedef string_const_iterator<TElement, TTraits, Allocator> MyType;
            typedef basic_string<TElement, TTraits, Allocator> MyString;
            typedef random_access_iterator_tag iterator_category;
            typedef TElement value_type;
            typedef typename Allocator::difference_type difference_type;
            typedef typename Allocator::const_pointer pointer;
            typedef typename Allocator::const_reference reference;
            typedef TElement* inner_type;
            typedef range_checked_iterator_tag checked_iterator_category;
            string_const_iterator() {
                myPtr_ = 0;
            }

            string_const_iterator(pointer ptr, const container_base_secure* pString) {
                {
                    if (!(pString == 0 || ptr != 0 && ((MyString*)pString)->myPtr_() <= ptr && ptr <= (((MyString*)pString)->myPtr_() + ((MyString*)pString)->mySize_))) {
                        (void)(((("pString == NULL || ptr != NULL && ((MyString *)pString)->myPtr_() <= ptr && ptr <= (((MyString *)pString)->myPtr_() + ((MyString *)pString)->mySize_)", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 86, 0, L"(\"pString == NULL || ptr != NULL && ((MyString *)pString)->myPtr_() <= ptr && ptr <= (((MyString *)pString)->myPtr_() + ((MyString *)pString)->mySize_)\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 86, 0);
                    }
                }
                this->adopt_(pString);
                myPtr_ = ptr;
            }

            reference  operator*() const {
                if (this->myContainer_ == 0 || myPtr_ == 0
                        || myPtr_ < ((MyString*)this->myContainer_)->myPtr_()
                        || ((MyString*)this->myContainer_)->myPtr_()
                        + ((MyString*)this->myContainer_)->mySize_ <= myPtr_) {
                    debug_message(L"string iterator not dereferencable", L"..\\ce\\include\\xstring", 122);
                    {
                        (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 123, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 123, 0);
                    }
                }

                return (*myPtr_);
            }

            pointer  operator->() const {
                return (&** this);
            }

            MyType&  operator++() {
                if (this->myContainer_ != ((const container_base_secure*) - 4)) {
                    {
                        if (!(this->_Has_container())) {
                            (void)(((("this->_Has_container()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 146, 0, L"(\"this->_Has_container()\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 146, 0);
                        }
                    }
                    {
                        if (!(myPtr_ < (((MyString*)this->myContainer_)->myPtr_() + ((MyString*)this->myContainer_)->mySize_))) {
                            (void)(((("myPtr_ < (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_)", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 147, 0, L"(\"myPtr_ < (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_)\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 147, 0);
                        }
                    }
                }

                ++myPtr_;
                return (*this);
            }

            MyType  operator++(int) {
                MyType temp = *this;
                ++*this;
                return (temp);
            }

            MyType&  operator--() {
                if (this->myContainer_ != ((const container_base_secure*) - 4)) {
                    {
                        if (!(this->_Has_container())) {
                            (void)(((("this->_Has_container()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 164, 0, L"(\"this->_Has_container()\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 164, 0);
                        }
                    }
                    {
                        if (!(myPtr_ > ((MyString*)this->myContainer_)->myPtr_())) {
                            (void)(((("myPtr_ > ((MyString *)this->myContainer_)->myPtr_()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 165, 0, L"(\"myPtr_ > ((MyString *)this->myContainer_)->myPtr_()\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 165, 0);
                        }
                    }
                }

                --myPtr_;
                return (*this);
            }

            MyType  operator--(int {
                MyType temp = *this;
                --*this;
                return (temp);
            }

            MyType&  operator+=(difference_type offset) {
                if (this->myContainer_ != ((const container_base_secure*) - 4)) {
                    {
                        if (!(this->_Has_container())) {
                            (void)(((("this->_Has_container()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 182, 0, L"(\"this->_Has_container()\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 182, 0);
                        }
                    }
                    {
                        if (!(myPtr_ + offset <= (((MyString*)this->myContainer_)->myPtr_() + ((MyString*)this->myContainer_)->mySize_) && myPtr_ + offset >= ((MyString*)this->myContainer_)->myPtr_())) {
                            (void)(((("myPtr_ + offset <= (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_) && myPtr_ + offset >= ((MyString *)this->myContainer_)->myPtr_()", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 185, 0, L"(\"myPtr_ + offset <= (((MyString *)this->myContainer_)->myPtr_() + ((MyString *)this->myContainer_)->mySize_) && myPtr_ + offset >= ((MyString *)this->myContainer_)->myPtr_()\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 185, 0);
                        }
                    }
                }

                myPtr_ += offset;
                return (*this);
            }

            MyType  operator+(difference_type offset) const {
                MyType temp = *this;
                return (temp += offset);
            }

            MyType&  operator-=(difference_type offset) {
                return (*this += -offset);
            }

            MyType  operator-(difference_type offset) const {
                MyType temp = *this;
                return (temp -= offset);
            }

            difference_type  operator-(const MyType& right) const {
                compat_(right);
                return (myPtr_ - right.myPtr_);
            }

            reference  operator[](difference_type offset) const {
                return (*(*this + offset));
            }

            bool  operator==(const MyType& right) const {
                compat_(right);
                return (myPtr_ == right.myPtr_);
            }

            bool  operator!=(const MyType& right) const {
                return (!(*this == right));
            }

            bool  operator<(const MyType& right) const {
                compat_(right);
                return (myPtr_ < right.myPtr_);
            }

            bool  operator>(const MyType& right) const {
                return (right < *this);
            }

            bool  operator<=(const MyType& right) const {
                return (!(right < *this));
            }

            bool  operator>=(const MyType& right) const {
                return (!(*this < right));
            }

            void  compat_(const MyType& right) const {
                if (this->myContainer_ == 0 || this->myContainer_ != right.myContainer_) {
                    debug_message(L"string iterators incompatible", L"..\\ce\\include\\xstring", 283);
                    {
                        (void)(((("Standard C++ Libraries Invalid Argument", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 284, 0, L"(\"Standard C++ Libraries Invalid Argument\", 0)")) || (__debugbreak(), 0));
                        ::_invalid_parameter(L"\"invalid argument\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 284, 0);
                    }
                }
            }

            static void __cdecl xLen() {
                MyString::xLen();
            }

            static void __cdecl xRange() {
                MyString::xRange();
            }

            static void __cdecl xInvalidArg() {
                MyString::xInvalidArg();
            }

            pointer myPtr_;
        };

        template<class TElement, class TTraits, class TAllocator> inline
        string_const_iterator<TElement, TTraits, TAllocator> __cdecl operator+(
            typename string_const_iterator<TElement, TTraits, TAllocator>::difference_type offset,
            string_const_iterator<TElement, TTraits, TAllocator> next) {
            return (next += offset);
        }

        template<class TElement, class TTraits, class Allocator>
        class string_iterator : public string_const_iterator<TElement, TTraits, Allocator> {
        public:
            typedef string_iterator<TElement, TTraits, Allocator> MyType;
            typedef string_const_iterator<TElement, TTraits, Allocator> MyBaseType;
            typedef random_access_iterator_tag iterator_category;
            typedef TElement value_type;
            typedef typename Allocator::difference_type difference_type;
            typedef typename Allocator::pointer pointer;
            typedef typename Allocator::reference reference;
            string_iterator() {
            }

            string_iterator(pointer ptr, const container_base_secure* pString)
                : MyBaseType(ptr, pString) {
            }

            reference  operator*() const {
                return ((reference)** (MyBaseType*)this);
            }

            pointer  operator->() const {
                return (&** this);
            }

            MyType&  operator++() {
                ++(*(MyBaseType*)this);
                return (*this);
            }

            MyType  operator++(int) {
                MyType temp = *this;
                ++*this;
                return (temp);
            }

            MyType&  operator--() {
                --(*(MyBaseType*)this);
                return (*this);
            }

            MyType  operator--(int) {
                MyType temp = *this;
                --*this;
                return (temp);
            }

            MyType&  operator+=(difference_type offset) {
                (*(MyBaseType*)this) += offset;
                return (*this);
            }

            MyType  operator+(difference_type offset) const {
                MyType temp = *this;
                return (temp += offset);
            }

            MyType&  operator-=(difference_type offset) {
                return (*this += -offset);
            }

            MyType  operator-(difference_type offset) const {
                MyType temp = *this;
                return (temp -= offset);
            }

            difference_type  operator-(const MyBaseType& right) const {
                return ((MyBaseType) * this - right);
            }

            reference  operator[](difference_type offset) const {
                return (*(*this + offset));
            }
        };

        template<class TElement, class TTraits, class Allocator> inline
        string_iterator<TElement, TTraits, Allocator>  operator+(
            typename string_iterator<TElement, TTraits, Allocator>::difference_type offset,
            string_iterator<TElement, TTraits, Allocator> next) {
            return (next += offset);
        }

        class  string_base : public container_base_secure {
        public:
            static void __cdecl xLen();
            static void __cdecl xRange();
            static void __cdecl xInvalidArg();
        };

        template<class T, class Allocator>
        class StringValue : public string_base {
        protected:
            typedef typename Allocator::template rebind<T>::other AlterType;
            StringValue(AlterType allocator = AlterType()) : alterValue_(allocator) {
            }
        public:
            StringValue(const StringValue& right) : alterValue_(right.alterValue_) {
                if (right.myFirstIterator_ == ((iterator_base*) - 3)) {
                    this->myFirstIterator_ = ((iterator_base*) - 3);
                }
            }
        protected:
            AlterType alterValue_;
        };

        template<class TElement, class TTraits, class TAllocator>
        class basic_string : public StringValue<TElement, TAllocator> {
        public:
            typedef basic_string<TElement, TTraits, TAllocator> MyType;
            typedef StringValue<TElement, TAllocator> MyBaseType;
            typedef typename MyBaseType::AlterType Allocator;
            typedef typename Allocator::size_type size_type;
            typedef typename Allocator::difference_type DifferenceType;
            typedef DifferenceType difference_type;
            typedef typename Allocator::pointer Ptr;
            typedef typename Allocator::const_pointer ConstPtr;
            typedef Ptr pointer;
            typedef ConstPtr const_pointer;
            typedef typename Allocator::reference Reference;
            typedef Reference reference;
            typedef typename Allocator::const_reference const_reference;
            typedef typename Allocator::value_type value_type;
            typedef string_iterator<TElement, TTraits, Allocator> iterator;
            typedef string_const_iterator<TElement, TTraits, Allocator> const_iterator;
            friend class string_const_iterator<TElement, TTraits, Allocator>;
            typedef std::reverse_iterator<iterator> reverse_iterator;
            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
            basic_string() : MyBaseType() {
                tidy_();
            }

            explicit  basic_string(const Allocator& allocator)
                : MyBaseType(allocator) {
                tidy_();
            }

            basic_string(const MyType& right, size_type rightOffset,
                         size_type count = npos)
                : MyBaseType() {
                if (right.myFirstIterator_ == ((iterator_base*) - 3)) {
                    this->myFirstIterator_ = ((iterator_base*) - 3);
                }

                tidy_();
                assign(right, rightOffset, count);
            }

            basic_string(const MyType& right, size_type rightOffset, size_type count,
                         const Allocator& allocator)
                : MyBaseType(allocator) {
                if (right.myFirstIterator_ == ((iterator_base*) - 3)) {
                    this->myFirstIterator_ = ((iterator_base*) - 3);
                }

                tidy_();
                assign(right, rightOffset, count);
            }

            basic_string(const TElement* ptr, size_type count)
                : MyBaseType() {
                tidy_();
                assign(ptr, count);
            }

            basic_string(const TElement* ptr, size_type count, const Allocator& allocator)
                : MyBaseType(allocator) {
                tidy_();
                assign(ptr, count);
            }

            basic_string(const TElement* ptr)
                : MyBaseType() {
                tidy_();
                assign(ptr);
            }

            basic_string(const TElement* ptr, const Allocator& allocator)
                : MyBaseType(allocator) {
                tidy_();
                assign(ptr);
            }

            basic_string(size_type count, TElement chr)
                : MyBaseType() {
                tidy_();
                assign(count, chr);
            }

            basic_string(size_type count, TElement chr, const Allocator& allocator)
                : MyBaseType(allocator) {
                tidy_();
                assign(count, chr);
            }

            template<class TIterator>
            basic_string(TIterator first, TIterator last)
                : MyBaseType() {
                tidy_();
                construct(first, last, iterator_category(first));
            }

            template<class TIterator>
            basic_string(TIterator first, TIterator last, const Allocator& allocator)
                : MyBaseType(allocator) {
                tidy_();
                construct(first, last, iterator_category(first));
            }

            basic_string(const_pointer first, const_pointer last)
                : MyBaseType() {
                debug_range(first, last, L"..\\ce\\include\\xstring", 707);
                tidy_();

                if (first != last)
                    assign(&*first, last - first);
            }

            basic_string(const_iterator first, const_iterator last)
                : MyBaseType() {
                debug_range(first, last, L"..\\ce\\include\\xstring", 718);
                tidy_();

                if (first != last)
                    assign(&*first, last - first);
            }

            basic_string(const MyType& right)
                : MyBaseType(right) {
                tidy_();
                assign(right, 0, npos);
            }

            template<class TIterator>
            void  construct(TIterator count, TIterator chr, int_iterator_tag) {
                assign((size_type)count, (TElement)chr);
            }

            template<class TIterator>
            void  construct(TIterator first, TIterator last, input_iterator_tag) {
                try {
                    for (; first != last; ++first)
                        append((size_type)1, (TElement)*first);
                } catch (...) {
                    tidy_(true);
                    throw;
                }
            }

            template<class TIterator>
            void  construct(TIterator first, TIterator last, forward_iterator_tag) {
                if (this->myFirstIterator_ != ((iterator_base*) - 3)) {
                    debug_range(first, last, L"..\\ce\\include\\xstring", 898);
                }

                size_type count = 0;
                distance(first, last, count);
                reserve(count);

                try {
                    for (; first != last; ++first)
                        append((size_type)1, (TElement)*first);
                } catch (...) {
                    tidy_(true);
                    throw;
                }
            }

            ~basic_string() {
                tidy_(true);
            }

            typedef TTraits traits_type;
            typedef Allocator allocator_type;
            static const size_type npos;
            MyType&  operator=(const MyType& right) {
                return (assign(right));
            }

            MyType&  operator=(const TElement* ptr) {
                return (assign(ptr));
            }

            MyType&  operator=(TElement chr) {
                return (assign(1, chr));
            }

            MyType&  operator+=(const MyType& right) {
                return (append(right));
            }

            MyType&  operator+=(const TElement* ptr) {
                return (append(ptr));
            }

            MyType&  operator+=(TElement chr) {
                return (append((size_type)1, chr));
            }

            MyType&  append(const MyType& right) {
                return (append(right, 0, npos));
            }

            MyType&  append(const MyType& right, size_type rightOffset, size_type count) {
                if (right.size() < rightOffset)
                    string_base::xRange();

                size_type number = right.size() - rightOffset;

                if (number < count)
                    count = number;

                if (npos - mySize_ <= count || mySize_ + count < mySize_)
                    string_base::xLen();

                if (0 < count && grow_(number = mySize_ + count)) {
                    traits_helper::copy_s<TTraits>(myPtr_() + mySize_, myRes_ - mySize_,
                                                   right.myPtr_() + rightOffset, count);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  append(const TElement* ptr, size_type count) {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 984);

                if (inside_(ptr))
                    return (append(*this, ptr - myPtr_(), count));

                if (npos - mySize_ <= count || mySize_ + count < mySize_)
                    string_base::xLen();

                size_type number;

                if (0 < count && grow_(number = mySize_ + count)) {
                    traits_helper::copy_s<TTraits>(myPtr_() + mySize_, myRes_ - mySize_, ptr, count);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  append(const TElement* ptr) {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1003);
                return (append(ptr, TTraits::length(ptr)));
            }

            MyType&  append(size_type count, TElement chr) {
                if (npos - mySize_ <= count)
                    string_base::xLen();

                size_type number;

                if (0 < count && grow_(number = mySize_ + count)) {
                    chAssign_(mySize_, count, chr);
                    eos_(number);
                }

                return (*this);
            }

            template<class TIterator>
            MyType&  append(TIterator first, TIterator last) {
                return (_Append(first, last, iterator_category(first)));
            }

            template<class TIterator>
            MyType&  _Append(TIterator count, TIterator chr, int_iterator_tag) {
                return (append((size_type)count, (TElement)chr));
            }

            template<class TIterator>
            MyType&  _Append(TIterator first, TIterator last, input_iterator_tag) {
                return (replace(end(), end(), first, last));
            }

            MyType&  append(const_pointer first, const_pointer last) {
                return (replace(end(), end(), first, last));
            }

            MyType&  append(const_iterator first, const_iterator last) {
                return (replace(end(), end(), first, last));
            }

            MyType&  assign(const MyType& right) {
                return (assign(right, 0, npos));
            }

            MyType&  assign(const MyType& right, size_type rightOffset, size_type count) {
                if (right.size() < rightOffset)
                    string_base::xRange();

                size_type number = right.size() - rightOffset;

                if (count < number)
                    number = count;

                if (this == &right)
                    erase((size_type)(rightOffset + number)), erase(0, rightOffset);
                else if (grow_(number)) {
                    traits_helper::copy_s<TTraits>(myPtr_(), myRes_, right.myPtr_() + rightOffset, number);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  assign(const TElement* ptr, size_type count) {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1078);

                if (inside_(ptr))
                    return (assign(*this, ptr - myPtr_(), count));

                if (grow_(count)) {
                    traits_helper::copy_s<TTraits>(myPtr_(), myRes_, ptr, count);
                    eos_(count);
                }

                return (*this);
            }

            MyType&  assign(const TElement* ptr) {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1094);
                return (assign(ptr, TTraits::length(ptr)));
            }

            MyType&  assign(size_type count, TElement chr) {
                if (count == npos)
                    string_base::xLen();

                if (grow_(count)) {
                    chAssign_(0, count, chr);
                    eos_(count);
                }

                return (*this);
            }

            template<class TIterator>
            MyType&  assign(TIterator first, TIterator last) {
                return (assign_(first, last, iterator_category(first)));
            }

            template<class TIterator>
            MyType&  assign_(TIterator count, TIterator chr, int_iterator_tag) {
                return (assign((size_type)count, (TElement)chr));
            }

            template<class TIterator>
            MyType&  assign_(TIterator first, TIterator last, input_iterator_tag) {
                return (replace(begin(), end(), first, last));
            }

            MyType&  assign(const_pointer first, const_pointer last) {
                return (replace(begin(), end(), first, last));
            }

            MyType&  assign(const_iterator first, const_iterator last) {
                return (replace(begin(), end(), first, last));
            }

            MyType&  insert(size_type offset, const MyType& right) {
                return (insert(offset, right, 0, npos));
            }

            MyType&  insert(size_type offset,
                            const MyType& right, size_type rightOffset, size_type count) {
                if (mySize_ < offset || right.size() < rightOffset)
                    string_base::xRange();

                size_type number = right.size() - rightOffset;

                if (number < count)
                    count = number;

                if (npos - mySize_ <= count)
                    string_base::xLen();

                if (0 < count && grow_(number = mySize_ + count)) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset, mySize_ - offset);

                    if (this == &right)
                        traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                       myPtr_() + (offset < rightOffset ? rightOffset + count : rightOffset),
                                                       count);
                    else
                        traits_helper::copy_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                       right.myPtr_() + rightOffset, count);

                    eos_(number);
                }

                return (*this);
            }
            MyType&  insert(size_type offset,
                            const TElement* ptr, size_type count) {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1177);

                if (inside_(ptr))
                    return (insert(offset, *this,
                                   ptr - myPtr_(), count));

                if (mySize_ < offset)
                    string_base::xRange();

                if (npos - mySize_ <= count)
                    string_base::xLen();

                size_type number;

                if (0 < count && grow_(number = mySize_ + count)) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset, mySize_ - offset);
                    traits_helper::copy_s<TTraits>(myPtr_() + offset, myRes_ - offset, ptr, count);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  insert(size_type offset, const TElement* ptr) {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1200);
                return (insert(offset, ptr, TTraits::length(ptr)));
            }

            MyType&  insert(size_type offset, size_type count, TElement chr) {
                if (mySize_ < offset)
                    string_base::xRange();

                if (npos - mySize_ <= count)
                    string_base::xLen();

                size_type number;

                if (0 < count && grow_(number = mySize_ + count)) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset, mySize_ - offset);
                    chAssign_(offset, count, chr);
                    eos_(number);
                }

                return (*this);
            }

            iterator  insert(const_iterator where) {
                return (insert(where, TElement()));
            }

            iterator  insert(const_iterator where, TElement chr) {
                size_type offset = pDifference_(where, begin());
                insert(offset, 1, chr);
                return (begin() + offset);
            }

            void  insert(const_iterator where, size_type count, TElement chr) {
                size_type offset = pDifference_(where, begin());
                insert(offset, count, chr);
            }

            template<class TIterator>
            void  insert(const_iterator where, TIterator first, TIterator last) {
                insert_(where, first, last, iterator_category(first));
            }

            template<class TIterator>
            void  insert_(const_iterator where, TIterator count, TIterator chr,
                          int_iterator_tag) {
                insert(where, (size_type)count, (TElement)chr);
            }

            template<class TIterator>
            void  insert_(const_iterator where, TIterator first, TIterator last,
                          input_iterator_tag) {
                replace(where, where, first, last);
            }

            void  insert(const_iterator where, const_pointer first, const_pointer last) {
                replace(where, where, first, last);
            }

            void  insert(const_iterator where, const_iterator first, const_iterator last) {
                replace(where, where, first, last);
            }

            MyType&  erase(size_type offset = 0,
                           size_type count = npos) {
                if (mySize_ < offset)
                    string_base::xRange();

                if (mySize_ - offset < count)
                    count = mySize_ - offset;

                if (0 < count) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset, myPtr_() + offset + count,
                                                   mySize_ - offset - count);
                    size_type newSize = mySize_ - count;
                    eos_(newSize);
                }

                return (*this);
            }

            iterator  erase(const_iterator where) {
                size_type count = pDifference_(where, begin());
                erase(count, 1);
                return (iterator(myPtr_() + count, this));
            }

            iterator  erase(const_iterator first, const_iterator last) {
                size_type count = pDifference_(first, begin());
                erase(count, pDifference_(last, first));
                return (iterator(myPtr_() + count, this));
            }

            void  clear() {
                erase(begin(), end());
            }

            MyType&  replace(size_type offset, size_type n0, const MyType& right) {
                return (replace(offset, n0, right, 0, npos));
            }

            MyType&  replace(size_type offset, size_type n0, const MyType& right, size_type rightOffset, size_type count) {
                if (mySize_ < offset || right.size() < rightOffset)
                    string_base::xRange();

                if (mySize_ - offset < n0)
                    n0 = mySize_ - offset;

                size_type number = right.size() - rightOffset;

                if (number < count)
                    count = number;

                if (npos - count <= mySize_ - n0)
                    string_base::xLen();

                size_type num = mySize_ - n0 - offset;
                size_type newSize = mySize_ + count - n0;

                if (mySize_ < newSize)
                    grow_(newSize);

                if (this != &right) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);
                    traits_helper::copy_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                   right.myPtr_() + rightOffset, count);
                } else if (count <= n0) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                   myPtr_() + rightOffset, count);
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);
                } else if (rightOffset <= offset) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);
                    traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                   myPtr_() + rightOffset, count);
                } else if (offset + n0 <= rightOffset) {
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);
                    traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                   myPtr_() + (rightOffset + count - n0), count);
                } else {
                    traits_helper::move_s<TTraits>(myPtr_() + offset, myRes_ - offset,
                                                   myPtr_() + rightOffset, n0);
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);
                    traits_helper::move_s<TTraits>(myPtr_() + offset + n0, myRes_ - offset - n0, myPtr_() + rightOffset + count,
                                                   count - n0);
                }

                eos_(newSize);
                return (*this);
            }

            MyType&  replace(size_type offset, size_type n0, const TElement* ptr, size_type count) {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1377);

                if (inside_(ptr))
                    return (replace(offset, n0, *this,
                                    ptr - myPtr_(), count));

                if (mySize_ < offset)
                    string_base::xRange();

                if (mySize_ - offset < n0)
                    n0 = mySize_ - offset;

                if (npos - count <= mySize_ - n0)
                    string_base::xLen();

                size_type num = mySize_ - n0 - offset;

                if (count < n0)
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);

                size_type number;

                if ((0 < count || 0 < n0) && grow_(number = mySize_ + count - n0)) {
                    if (n0 < count)
                        traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                       myPtr_() + offset + n0, num);

                    traits_helper::copy_s<TTraits>(myPtr_() + offset, myRes_ - offset, ptr, count);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  replace(size_type offset, size_type n0, const TElement* ptr) {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1408);
                return (replace(offset, n0, ptr, TTraits::length(ptr)));
            }

            MyType&  replace(size_type offset, size_type n0, size_type count, TElement chr) {
                if (mySize_ < offset)
                    string_base::xRange();

                if (mySize_ - offset < n0)
                    n0 = mySize_ - offset;

                if (npos - count <= mySize_ - n0)
                    string_base::xLen();

                size_type num = mySize_ - n0 - offset;

                if (count < n0)
                    traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                   myPtr_() + offset + n0, num);

                size_type number;

                if ((0 < count || 0 < n0) && grow_(number = mySize_ + count - n0)) {
                    if (n0 < count)
                        traits_helper::move_s<TTraits>(myPtr_() + offset + count, myRes_ - offset - count,
                                                       myPtr_() + offset + n0, num);

                    chAssign_(offset, count, chr);
                    eos_(number);
                }

                return (*this);
            }

            MyType&  replace(const_iterator first, const_iterator last, const MyType& right) {
                return (replace(
                            pDifference_(first, begin()), pDifference_(last, first), right));
            }

            MyType&  replace(const_iterator first, const_iterator last, const TElement* ptr,
                             size_type count) {
                return (replace(
                            pDifference_(first, begin()), pDifference_(last, first), ptr, count));
            }

            MyType&  replace(const_iterator first, const_iterator last, const TElement* ptr) {
                return (replace(
                            pDifference_(first, begin()), pDifference_(last, first), ptr));
            }

            MyType&  replace(const_iterator first, const_iterator last,
                             size_type count, TElement chr) {
                return (replace(
                            pDifference_(first, begin()), pDifference_(last, first), count, chr));
            }

            template<class TIterator>
            MyType&  replace(const_iterator first, const_iterator last,
                             TIterator first2, TIterator last2) {
                return (replace(first, last,
                                first2, last2, iterator_category(first2)));
            }

            template<class TIterator>
            MyType&  replace(const_iterator first, const_iterator last,
                             TIterator count, TIterator chr, int_iterator_tag) {
                return (replace(first, last, (size_type)count, (TElement)chr));
            }

            template<class TIterator>
            MyType&  replace(const_iterator first, const_iterator last,
                             TIterator first2, TIterator last2, input_iterator_tag) {
                MyType right(first2, last2);
                replace(first, last, right);
                return (*this);
            }

            MyType&  replace(const_iterator first, const_iterator last,
                             const_pointer first2, const_pointer last2) {
                if (first2 == last2)
                    erase(pDifference_(first, begin()), pDifference_(last, first));
                else
                    replace(pDifference_(first, begin()), pDifference_(last, first),
                            &*first2, last2 - first2);

                return (*this);
            }

            MyType&  replace(const_iterator first, const_iterator last,
                             const_iterator first2, const_iterator last2) {
                if (first2 == last2)
                    erase(pDifference_(first, begin()), pDifference_(last, first));
                else
                    replace(pDifference_(first, begin()), pDifference_(last, first),
                            &*first2, last2 - first2);

                return (*this);
            }

            iterator  begin() {
                return (iterator(myPtr_(), this));
            }

            const_iterator  begin() const {
                return (const_iterator(myPtr_(), this));
            }

            iterator  end() {
                return (iterator(myPtr_() + mySize_, this));
            }

            const_iterator  end() const {
                return (const_iterator(myPtr_() + mySize_, this));
            }

            reverse_iterator  rbegin() {
                return (reverse_iterator(end()));
            }

            const_reverse_iterator  rbegin() const {
                return (const_reverse_iterator(end()));
            }

            reverse_iterator  rend() {
                return (reverse_iterator(begin()));
            }

            const_reverse_iterator  rend() const {
                return (const_reverse_iterator(begin()));
            }

            reference  at(size_type offset) {
                if (mySize_ <= offset)
                    string_base::xRange();

                return (myPtr_()[offset]);
            }

            const_reference  at(size_type offset) const {
                if (mySize_ <= offset)
                    string_base::xRange();

                return (myPtr_()[offset]);
            }

            reference  operator[](size_type offset) {
                if (this->myFirstIterator_ != ((iterator_base*) - 3)) {
                    if (mySize_ < offset) {
                        debug_message(L"string subscript out of range", L"..\\ce\\include\\xstring", 1575);
                        {
                            (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 1576, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 1576, 0);
                        }
                    }
                }

                return (myPtr_()[offset]);
            }

            const_reference  operator[](size_type offset) const {
                if (this->myFirstIterator_ != ((iterator_base*) - 3)) {
                    if (mySize_ < offset) {
                        debug_message(L"string subscript out of range", L"..\\ce\\include\\xstring", 1595);
                        {
                            (void)(((("Standard C++ Libraries Out of Range", 0))) || (1 != _CrtDbgReportW(2, L"..\\ce\\include\\xstring", 1596, 0, L"(\"Standard C++ Libraries Out of Range\", 0)")) || (__debugbreak(), 0));
                            ::_invalid_parameter(L"\"out of range\"", __LPREFIX(__FUNCTION__), L"..\\ce\\include\\xstring", 1596, 0);
                        }
                    }
                }

                return (myPtr_()[offset]);
            }

            void  push_back(TElement chr) {
                insert(end(), chr);
            }

            const TElement* c_str() const {
                return (myPtr_());
            }

            const TElement* data() const {
                return (c_str());
            }

            size_type  length() const {
                return (mySize_);
            }

            size_type  size() const {
                return (mySize_);
            }

            size_type  maxSize() const {
                size_type number = MyBaseType::alterValue_.maxSize();
                return (number <= 1 ? 1 : number - 1);
            }

            void  resize(size_type newSize) {
                resize(newSize, TElement());
            }

            void  resize(size_type newSize, TElement chr) {
                if (newSize <= mySize_)
                    erase(newSize);
                else
                    append(newSize - mySize_, chr);
            }

            size_type  capacity() const {
                return (myRes_);
            }

            void  reserve(size_type newCap = 0) {
                if (mySize_ <= newCap && myRes_ != newCap) {
                    size_type size = mySize_;

                    if (grow_(newCap, true))
                        eos_(size);
                }
            }

            bool  empty() const {
                return (mySize_ == 0);
            }

            __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
            size_type  copy(TElement* destination,
                            size_type count, size_type offset = 0) const {
                if (count != 0)
                    debug_pointer(destination, L"..\\ce\\include\\xstring", 1725);

                return copy_s(destination, count, count, offset);
            }

            size_type  copy_s(TElement* destination, size_type _DestSize,
                              size_type count, size_type offset = 0) const {
                debug_pointer(destination, L"..\\ce\\include\\xstring", 1735);

                if (mySize_ < offset)
                    string_base::xRange();

                if (mySize_ - offset < count)
                    count = mySize_ - offset;

                traits_helper::copy_s<TTraits>(destination, _DestSize, myPtr_() + offset, count);
                return (count);
            }

            void  swap(MyType& right) {
                if (this == &right)
                    ;
                else if (MyBaseType::alterValue_ == right.alterValue_) {
                    this->_Swap_all(right);
                    BoxType _Tbx = box_;
                    box_ = right.box_, right.box_ = _Tbx;
                    size_type length = mySize_;
                    mySize_ = right.mySize_, right.mySize_ = length;
                    size_type res = myRes_;
                    myRes_ = right.myRes_, right.myRes_ = res;
                } else {
                    MyType temp = *this;
                    *this = right;
                    right = temp;
                }
            }

            size_type  find(const MyType& right, size_type offset = 0) const {
                return (find(right.myPtr_(), offset, right.size()));
            }

            size_type  find(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1784);

                if (count == 0 && offset <= mySize_)
                    return (offset);

                size_type num;

                if (offset < mySize_ && count <= (num = mySize_ - offset)) {
                    const TElement* uPtr, *vptr;

                    for (num -= count - 1, vptr = myPtr_() + offset;
                            (uPtr = TTraits::find(vptr, num, *ptr)) != 0;
                            num -= uPtr - vptr + 1, vptr = uPtr + 1)
                        if (TTraits::compare(uPtr, ptr, count) == 0)
                            return (uPtr - myPtr_());
                }

                return (npos);
            }

            size_type  find(const TElement* ptr, size_type offset = 0) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1806);
                return (find(ptr, offset, TTraits::length(ptr)));
            }

            size_type  find(TElement chr, size_type offset = 0) const {
                return (find((const TElement*)&chr, offset, 1));
            }

            size_type  rfind(const MyType& right, size_type offset = npos) const {
                return (rfind(right.myPtr_(), offset, right.size()));
            }

            size_type  rfind(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1826);

                if (count == 0)
                    return (offset < mySize_ ? offset : mySize_);

                if (count <= mySize_) {
                    const TElement* uPtr = myPtr_() +
                                           (offset < mySize_ - count ? offset : mySize_ - count);

                    for (; ; --uPtr)
                        if (TTraits::eq(*uPtr, *ptr)
                                && TTraits::compare(uPtr, ptr, count) == 0)
                            return (uPtr - myPtr_());
                        else if (uPtr == myPtr_())
                            break;
                }

                return (npos);
            }

            size_type  rfind(const TElement* ptr, size_type offset = npos) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1848);
                return (rfind(ptr, offset, TTraits::length(ptr)));
            }

            size_type  rfind(TElement chr, size_type offset = npos) const {
                return (rfind((const TElement*)&chr, offset, 1));
            }

            size_type  find_first_of(const MyType& right,
                                     size_type offset = 0) const {
                return (find_first_of(right.myPtr_(), offset, right.size()));
            }

            size_type  find_first_of(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1869);

                if (0 < count && offset < mySize_) {
                    const TElement* const vptr = myPtr_() + mySize_;

                    for (const TElement* uPtr = myPtr_() + offset; uPtr < vptr; ++uPtr)
                        if (TTraits::find(ptr, count, *uPtr) != 0)
                            return (uPtr - myPtr_());
                }

                return (npos);
            }

            size_type  find_first_of(const TElement* ptr, size_type offset = 0) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1885);
                return (find_first_of(ptr, offset, TTraits::length(ptr)));
            }

            size_type  find_first_of(TElement chr, size_type offset = 0) const {
                return (find((const TElement*)&chr, offset, 1));
            }

            size_type  find_last_of(const MyType& right, size_type offset = npos) const {
                return (find_last_of(right.myPtr_(), offset, right.size()));
            }

            size_type  find_last_of(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1906);

                if (0 < count && 0 < mySize_)
                    for (const TElement* uPtr = myPtr_()
                                                + (offset < mySize_ ? offset : mySize_ - 1); ; --uPtr)
                        if (TTraits::find(ptr, count, *uPtr) != 0)
                            return (uPtr - myPtr_());
                        else if (uPtr == myPtr_())
                            break;

                return (npos);
            }

            size_type  find_last_of(const TElement* ptr, size_type offset = npos) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1923);
                return (find_last_of(ptr, offset, TTraits::length(ptr)));
            }

            size_type  find_last_of(TElement chr, size_type offset = npos) const {
                return (rfind((const TElement*)&chr, offset, 1));
            }

            size_type  find_first_not_of(const MyType& right, size_type offset = 0) const {
                return (find_first_not_of(right.myPtr_(), offset, right.size()));
            }

            size_type  find_first_not_of(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1945);

                if (offset < mySize_) {
                    const TElement* const vptr = myPtr_() + mySize_;

                    for (const TElement* uPtr = myPtr_() + offset; uPtr < vptr; ++uPtr)
                        if (TTraits::find(ptr, count, *uPtr) == 0)
                            return (uPtr - myPtr_());
                }

                return (npos);
            }

            size_type  find_first_not_of(const TElement* ptr, size_type offset = 0) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1961);
                return (find_first_not_of(ptr, offset, TTraits::length(ptr)));
            }

            size_type  find_first_not_of(TElement chr, size_type offset = 0) const {
                return (find_first_not_of((const TElement*)&chr, offset, 1));
            }

            size_type  find_last_not_of(const MyType& right, size_type offset = npos) const {
                return (find_last_not_of(right.myPtr_(), offset, right.size()));
            }

            size_type  find_last_not_of(const TElement* ptr, size_type offset, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 1982);

                if (0 < mySize_)
                    for (const TElement* uPtr = myPtr_()
                                                + (offset < mySize_ ? offset : mySize_ - 1); ; --uPtr)
                        if (TTraits::find(ptr, count, *uPtr) == 0)
                            return (uPtr - myPtr_());
                        else if (uPtr == myPtr_())
                            break;

                return (npos);
            }

            size_type  find_last_not_of(const TElement* ptr, size_type offset = npos) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 1998);
                return (find_last_not_of(ptr, offset, TTraits::length(ptr)));
            }

            size_type  find_last_not_of(TElement chr, size_type offset = npos) const {
                return (find_last_not_of((const TElement*)&chr, offset, 1));
            }

            MyType  substr(size_type offset = 0, size_type count = npos) const {
                return (MyType(*this, offset, count, get_allocator()));
            }

            int  compare(const MyType& right) const {
                return (compare(0, mySize_, right.myPtr_(), right.size()));
            }

            int  compare(size_type offset, size_type n0, const MyType& right) const {
                return (compare(offset, n0, right, 0, npos));
            }

            int  compare(size_type offset, size_type n0, const MyType& right, size_type rightOffset, size_type count) const {
                if (right.size() < rightOffset)
                    string_base::xRange();

                if (right.mySize_ - rightOffset < count)
                    count = right.mySize_ - rightOffset;

                return (compare(offset, n0, right.myPtr_() + rightOffset, count));
            }

            int  compare(const TElement* ptr) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 2036);
                return (compare(0, mySize_, ptr, TTraits::length(ptr)));
            }

            int  compare(size_type offset, size_type n0, const TElement* ptr) const {
                debug_pointer(ptr, L"..\\ce\\include\\xstring", 2042);
                return (compare(offset, n0, ptr, TTraits::length(ptr)));
            }

            int  compare(size_type offset, size_type n0, const TElement* ptr, size_type count) const {
                if (count != 0)
                    debug_pointer(ptr, L"..\\ce\\include\\xstring", 2052);

                if (mySize_ < offset)
                    string_base::xRange();

                if (mySize_ - offset < n0)
                    n0 = mySize_ - offset;

                size_type ans = TTraits::compare(myPtr_() + offset, ptr, n0 < count ? n0 : count);
                return (ans != 0 ? (int)ans : n0 < count ? -1 : n0 == count ? 0 : +1);
            }

            allocator_type  get_allocator() const {
                return (MyBaseType::alterValue_);
            }

            enum {
                BufferSize = 16 / sizeof(TElement) < 1 ? 1 : 16 / sizeof(TElement)
            };
        protected:
            enum {
                AllocMask = sizeof(TElement) <= 1 ? 15
                : sizeof(TElement) <= 2 ? 7
                : sizeof(TElement) <= 4 ? 3
                : sizeof(TElement) <= 8 ? 1 : 0
            };

            void  chAssign_(size_type offset, size_type count, TElement chr) {
                if (count == 1)
                    TTraits::assign(*(myPtr_() + offset), chr);
                else
                    TTraits::assign(myPtr_() + offset, count, chr);
            }

            void  copy_(size_type newSize, size_type oldLength) {
                size_type newRes = newSize | AllocMask;

                if (maxSize() < newRes)
                    newRes = newSize;
                else if (newRes / 3 < myRes_ / 2
                         && myRes_ <= maxSize() - myRes_ / 2)
                    newRes = myRes_ + myRes_ / 2;

                TElement* ptr = 0;

                try {
                    ptr = MyBaseType::alterValue_.allocate(newRes + 1);
                } catch (...) {
                    newRes = newSize;

                    try {
                        ptr = MyBaseType::alterValue_.allocate(newRes + 1);
                    } catch (...) {
                        tidy_(true);
                        throw;
                    }
                }

                if (0 < oldLength)
                    traits_helper::copy_s<TTraits>(ptr, newRes + 1, myPtr_(), oldLength);

                tidy_(true);
                box_.ptr = ptr;
                myRes_ = newRes;
                eos_(oldLength);
            }

            void  eos_(size_type newSize) {
                TTraits::assign(myPtr_()[mySize_ = newSize], TElement());
            }

            bool  grow_(size_type newSize,
                        bool isTrim = false) {
                if (maxSize() < newSize)
                    string_base::xLen();

                if (myRes_ < newSize)
                    copy_(newSize, mySize_);
                else if (isTrim && newSize < BufferSize)
                    tidy_(true,
                          newSize < mySize_ ? newSize : mySize_);
                else if (newSize == 0)
                    eos_(0);

                return (0 < newSize);
            }

            bool  inside_(const TElement* ptr) {
                if (ptr == 0 || ptr < myPtr_() || myPtr_() + mySize_ <= ptr)
                    return (false);
                else
                    return (true);
            }

            static size_type __cdecl pDifference_(const_iterator p2,
                                                  const_iterator p1) {
                return ((p2).myPtr_ == 0 ? 0 : p2 - p1);
            }

            void  tidy_(bool built = false,
                        size_type newSize = 0) {
                if (!built)
                    ;
                else if (BufferSize <= myRes_) {
                    TElement* ptr = box_.ptr;

                    if (0 < newSize)
                        traits_helper::copy_s<TTraits>(box_.buffer_, BufferSize, ptr, newSize);

                    MyBaseType::alterValue_.deallocate(ptr, myRes_ + 1);
                }

                myRes_ = BufferSize - 1;
                eos_(newSize);
            }

            union BoxType {
                TElement buffer_[BufferSize];
                TElement* ptr;
            } box_;

            TElement* myPtr_() {
                return (BufferSize <= myRes_ ? box_.ptr : box_.buffer_);
            }

            const TElement* myPtr_() const {
                return (BufferSize <= myRes_ ? box_.ptr : box_.buffer_);
            }
            size_type mySize_;
            size_type myRes_;
        };

        template<class TElement, class TTraits, class TAllocator>
        class move_operation_category<basic_string<TElement, TTraits, TAllocator> > {
        public:
            typedef swapmove_tag move_category;
        };

        template<class TElement, class TTraits, class Allocator>
        const typename basic_string<TElement, TTraits, Allocator>::size_type
        basic_string<TElement, TTraits, Allocator>::npos =
            (typename basic_string<TElement, TTraits, Allocator>::size_type)(-1);

        template<class TElement, class TTraits, class TAllocator> inline
        void __cdecl swap(basic_string<TElement, TTraits, TAllocator>& left,
                          basic_string<TElement, TTraits, TAllocator>& right) {
            left.swap(right);
        }

        typedef basic_string<char, char_traits<char>, allocator<char> > string;
        typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;
    }

    namespace std {
        template<class TArg, class TResult>
        struct unary_function {
            typedef TArg argument_type;
            typedef TResult result_type;
        };

        template<class TArg1, class TArg2, class TResult>
        struct binary_function {
            typedef TArg1 first_argument_type;
            typedef TArg2 second_argument_type;
            typedef TResult result_type;
        };

        template<class T>
        struct plus : public binary_function<T, T, T> {
            T operator()(const T& left, const T& right) const {
                return (left + right);
            }
        };

        template<class T>
        struct minus : public binary_function<T, T, T> {
            T operator()(const T& left, const T& right) const {
                return (left - right);
            }
        };

        template<class T>
        struct multiplies : public binary_function<T, T, T> {
            T operator()(const T& left, const T& right) const {
                return (left * right);
            }
        };

        template<class T>
        struct divides : public binary_function<T, T, T> {
            T operator()(const T& left, const T& right) const {
                return (left / right);
            }
        };

        template<class T>
        struct modulus : public binary_function<T, T, T> {
            T operator()(const T& left, const T& right) const {
                return (left % right);
            }
        };

        template<class T>
        struct negate : public unary_function<T, T> {
            T operator()(const T& left) const {
                return (-left);
            }
        };

        template<class T>
        struct equal_to : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left == right);
            }
        };

        template<class T>
        struct not_equal_to : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const  {
                return (left != right);
            }
        };

        template<class T>
        struct greater : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left > right);
            }
        };

        template<class T>
        struct less : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left < right);
            }
        };

        template<class T>
        struct greater_equal : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left >= right);
            }
        };

        template<class T>
        struct less_equal : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left <= right);
            }
        };

        template<class T>
        struct logical_and : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left && right);
            }
        };

        template<class T>
        struct logical_or : public binary_function<T, T, bool> {
            bool operator()(const T& left, const T& right) const {
                return (left || right);
            }
        };

        template<class T>
        struct logical_not : public unary_function<T, bool> {
            bool operator()(const T& left) const {
                return (!left);
            }
        };

        template<class TFunction1>
        class unary_negate : public unary_function<typename TFunction1::argument_type, bool> {
        public:
            explicit unary_negate(const TFunction1& function)
                : functor_(function) {
            }

            bool operator()(const typename TFunction1::argument_type& left) const {
                return (!functor_(left));
            }
        protected:
            TFunction1 functor_;
        };

        template<class TFunction1> inline
        unary_negate<TFunction1> not1(const TFunction1& function) {
            return (std::unary_negate<TFunction1>(function));
        }

        template<class TFunction2>
        class binary_negate
                : public binary_function<typename TFunction2::first_argument_type, typename TFunction2::second_argument_type, bool> {
        public:
            explicit binary_negate(const TFunction2& function)
                : functor_(function) {
            }

            bool operator()(const typename TFunction2::first_argument_type& left,
                            const typename TFunction2::second_argument_type& right) const {
                return (!functor_(left, right));
            }
        protected:
            TFunction2 functor_;
        };

        template<class TFunction2> inline
        binary_negate<TFunction2> not2(const TFunction2& function) {
            return (std::binary_negate<TFunction2>(function));
        }

        template<class TFunction2>
        class binder1st
                : public unary_function<typename TFunction2::second_argument_type, typename TFunction2::result_type> {
        public:
            typedef unary_function < typename TFunction2::second_argument_type,
                    typename TFunction2::result_type > base;
            typedef typename base::argument_type argument_type;
            typedef typename base::result_type result_type;
            binder1st(const TFunction2& function, const typename TFunction2::first_argument_type& left)
                : op_(function), value_(left) {
            }

            result_type operator()(const argument_type& right) const {
                return (op_(value_, right));
            }

            result_type operator()(argument_type& right) const {
                return (op_(value_, right));
            }
        protected:
            TFunction2 op_;
            typename TFunction2::first_argument_type value_;
        };

        template<class TFunction2, class T> inline
        binder1st<TFunction2> bind1st(const TFunction2& function, const T& left) {
            typename TFunction2::first_argument_type value(left);
            return (std::binder1st<TFunction2>(function, value));
        }

        template<class TFunction2>
        class binder2nd
                : public unary_function<typename TFunction2::first_argument_type, typename TFunction2::result_type> {
        public:
            typedef unary_function<typename TFunction2::first_argument_type, typename TFunction2::result_type> base;
            typedef typename base::argument_type argument_type;
            typedef typename base::result_type result_type;
            binder2nd(const TFunction2& function, const typename TFunction2::second_argument_type& right)
                : op_(function), value_(right) {
            }

            result_type operator()(const argument_type& left) const {
                return (op_(left, value_));
            }

            result_type operator()(argument_type& left) const {
                return (op_(left, value_));
            }
        protected:
            TFunction2 op_;
            typename TFunction2::second_argument_type value_;
        };

        template<class TFunction2, class T> inline
        binder2nd<TFunction2> bind2nd(const TFunction2& function, const T& right) {
            typename TFunction2::second_argument_type value(right);
            return (std::binder2nd<TFunction2>(function, value));
        }

        template < class TArg, class TResult, class TFunction = TResult(*)(TArg) >
        class pointer_to_unary_function
                : public unary_function<TArg, TResult> {
        public:
            explicit pointer_to_unary_function(TFunction left)
                : function_(left) {
            }

            TResult operator()(TArg left) const {
                return (function_(left));
            }
        protected:
            TFunction function_;
        };

        template < class TArg1, class TArg2, class TResult, class TFunction = TResult(*)(TArg1, TArg2) >
        class pointer_to_binary_function
                : public binary_function<TArg1, TArg2, TResult> {
        public:
            explicit pointer_to_binary_function(TFunction left)
                : function_(left) {
            }

            TResult operator()(TArg1 left, TArg2 right) const {
                return (function_(left, right));
            }
        protected:
            TFunction function_;
        };

        template<class TArg, class TResult> inline
        pointer_to_unary_function<TArg, TResult, TResult(__cdecl*)(TArg)>
        ptr_fun(TResult(__cdecl* left)(TArg)) {
            return (std::pointer_to_unary_function<TArg, TResult, TResult(__cdecl*)(TArg)>(left));
        }

        template<class TArg1, class TArg2, class TResult> inline
        pointer_to_binary_function<TArg1, TArg2, TResult, TResult(__cdecl*)(TArg1, TArg2)>
        ptr_fun(TResult(__cdecl* left)(TArg1, TArg2)) {
            return (std::pointer_to_binary_function<TArg1, TArg2, TResult, TResult(__cdecl*)(TArg1, TArg2)>(left));
        }

        template<class TResult, class T>
        class mem_fun_t
                : public unary_function<T*, TResult> {
        public:
            explicit mem_fun_t(TResult(T::*memberFunction)())
                : memberFunction_(memberFunction) {
            }

            TResult operator()(T* pLeft) const {
                return ((pLeft->*memberFunction_)());
            }
        private:
            TResult(T::*memberFunction_)();
        };

        template<class TResult, class T, class TArg>
        class mem_fun1_t
                : public binary_function<T*, TArg, TResult> {
        public:
            explicit mem_fun1_t(TResult(T::*memberFunction)(TArg))
                : memberFunction_(memberFunction) {
            }

            TResult operator()(T* pLeft, TArg right) const {
                return ((pLeft->*memberFunction_)(right));
            }
        private:
            TResult(T::*memberFunction_)(TArg);
        };

        template<class TResult, class T>
        class const_mem_fun_t
                : public unary_function<const T*, TResult> {
        public:
            explicit const_mem_fun_t(TResult(T::*memberFunction)() const)
                : memberFunction_(memberFunction) {
            }

            TResult operator()(const T* pLeft) const {
                return ((pLeft->*memberFunction_)());
            }
        private:
            TResult(T::*memberFunction_)() const;
        };

        template<class TResult, class T, class TArg>
        class const_mem_fun1_t
                : public binary_function<const T*, TArg, TResult> {
        public:
            explicit const_mem_fun1_t(TResult(T::*memberFunction)(TArg) const)
                : memberFunction_(memberFunction) {
            }

            TResult operator()(const T* pLeft, TArg right) const {
                return ((pLeft->*memberFunction_)(right));
            }
        private:
            TResult(T::*memberFunction_)(TArg) const;
        };

        template<class TResult, class T> inline
        mem_fun_t<TResult, T> mem_fun(TResult(T::*memberFunction)()) {
            return (std::mem_fun_t<TResult, T>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        mem_fun1_t<TResult, T, TArg> mem_fun(TResult(T::*memberFunction)(TArg)) {
            return (std::mem_fun1_t<TResult, T, TArg>(memberFunction));
        }

        template<class TResult, class T> inline
        const_mem_fun_t<TResult, T>
        mem_fun(TResult(T::*memberFunction)() const) {
            return (std::const_mem_fun_t<TResult, T>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        const_mem_fun1_t<TResult, T, TArg>
        mem_fun(TResult(T::*memberFunction)(TArg) const) {
            return (std::const_mem_fun1_t<TResult, T, TArg>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        mem_fun1_t<TResult, T, TArg> mem_fun1(TResult(T::*memberFunction)(TArg)) {
            return (std::mem_fun1_t<TResult, T, TArg>(memberFunction));
        }

        template<class TResult, class T>
        class mem_fun_ref_t
                : public unary_function<T, TResult> {
        public:
            explicit mem_fun_ref_t(TResult(T::*memberFunction)())
                : memberFunction_(memberFunction) {
            }

            TResult operator()(T& left) const {
                return ((left.*memberFunction_)());
            }
        private:
            TResult(T::*memberFunction_)();
        };

        template<class TResult, class T, class TArg>
        class mem_fun1_ref_t
                : public binary_function<T, TArg, TResult> {
        public:
            explicit mem_fun1_ref_t(TResult(T::*memberFunction)(TArg))
                : memberFunction_(memberFunction) {
            }

            TResult operator()(T& left, TArg right) const {
                return ((left.*memberFunction_)(right));
            }
        private:
            TResult(T::*memberFunction_)(TArg);
        };

        template<class TResult, class T>
        class const_mem_fun_ref_t
                : public unary_function<T, TResult> {
        public:
            explicit const_mem_fun_ref_t(TResult(T::*memberFunction)() const)
                : memberFunction_(memberFunction) {
            }

            TResult operator()(const T& left) const {
                return ((left.*memberFunction_)());
            }
        private:
            TResult(T::*memberFunction_)() const;
        };

        template<class TResult, class T, class TArg>
        class const_mem_fun1_ref_t
                : public binary_function<T, TArg, TResult> {
        public:
            explicit const_mem_fun1_ref_t(TResult(T::*memberFunction)(TArg) const)
                : memberFunction_(memberFunction) {
            }

            TResult operator()(const T& left, TArg right) const {
                return ((left.*memberFunction_)(right));
            }
        private:
            TResult(T::*memberFunction_)(TArg) const;
        };

        template<class TResult, class T> inline
        mem_fun_ref_t<TResult, T> mem_fun_ref(TResult(T::*memberFunction)()) {
            return (std::mem_fun_ref_t<TResult, T>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        mem_fun1_ref_t<TResult, T, TArg>
        mem_fun_ref(TResult(T::*memberFunction)(TArg)) {
            return (std::mem_fun1_ref_t<TResult, T, TArg>(memberFunction));
        }

        template<class TResult, class T> inline
        const_mem_fun_ref_t<TResult, T>
        mem_fun_ref(TResult(T::*memberFunction)() const) {
            return (std::const_mem_fun_ref_t<TResult, T>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        const_mem_fun1_ref_t<TResult, T, TArg>
        mem_fun_ref(TResult(T::*memberFunction)(TArg) const) {
            return (std::const_mem_fun1_ref_t<TResult, T, TArg>(memberFunction));
        }

        template<class TResult, class T, class TArg> inline
        mem_fun1_ref_t<TResult, T, TArg> mem_fun1_ref(TResult(T::*memberFunction)(TArg)) {
            return (std::mem_fun1_ref_t<TResult, T, TArg>(memberFunction));
        }
    }

    namespace std {
        namespace tr1 {
            template<class TKey>
            class hash
                    : public unary_function<TKey, size_t> {
            public:
                size_t operator()(const TKey& keyValue) const {
                    ldiv_t qRem = ldiv((size_t)keyValue, 127773);
                    qRem.rem = 16807 * qRem.rem - 2836 * qRem.quot;

                    if (qRem.rem < 0)
                        qRem.rem += 2147483647;

                    return ((size_t)qRem.rem);
                }
            };

            template<>
            class hash<std::string> {
            public:
                typedef std::string TKey;
                size_t operator()(const TKey& keyValue) const {
                    size_t value = 2166136261U;
                    size_t first = 0;
                    size_t last = keyValue.size();
                    size_t stride = 1 + last / 10;

                    if (stride < last)
                        last -= stride;

                    for (; first < last; first += stride)
                        value = 16777619U * value ^(size_t)keyValue[first];

                    return (value);
                }
            };

            template<>
            class hash<std::wstring> {
            public:
                typedef std::wstring TKey;
                size_t operator()(const TKey& keyValue) const {
                    size_t value = 2166136261U;
                    size_t first = 0;
                    size_t last = keyValue.size();
                    size_t stride = 1 + last / 10;

                    if (stride < last)
                        last -= stride;

                    for (; first < last; first += stride)
                        value = 16777619U * value ^(size_t)keyValue[first];

                    return (value);
                }
            };
        }
    }

    namespace std {
        const int SORT_MAX = 32;
        template<class TInputInterator, class TFunction1> inline
        TFunction1 for_each(TInputInterator first, TInputInterator last, TFunction1 function) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 24);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 25);
            typename ::std::checked_iterator_base_helper<TInputInterator>::checked_iterator_base_type checkFirst(::std::checked_base(first));
            typename ::std::checked_iterator_base_helper<TInputInterator>::checked_iterator_base_type checkLast(::std::checked_base(last));

            for (; checkFirst != checkLast; ++checkFirst)
                function(*checkFirst);

            return (function);
        }

        template<class TInputInterator, class T>
        inline
        TInputInterator find(TInputInterator first, TInputInterator last, const T& value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 38);

            for (; first != last; ++first)
                if (*first == value)
                    break;

            return (first);
        }

        inline const char* find(const char* first, const char* last, int value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 47);
            first = (const char*)::memchr(first, value, last - first);
            return (first == 0 ? last : first);
        }

        inline const signed char* find(const signed char* first, const signed char* last, int value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 55);
            first = (const signed char*)::memchr(first, value,
                                                 last - first);
            return (first == 0 ? last : first);
        }

        inline const unsigned char* find(const unsigned char* first, const unsigned char* last, int value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 64);
            first = (const unsigned char*)::memchr(first, value, last - first);
            return (first == 0 ? last : first);
        }

        template<class TInputInterator, class T>
        inline
        TInputInterator find(TInputInterator first, TInputInterator last, const T& value) {
            ::std::checked_assign_from_base(first, find(::std::checked_base(first), ::std::checked_base(last), value));
            return (first);
        }

        template<class TInputInterator, class TPredication> inline
        TInputInterator find_if(TInputInterator first, TInputInterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 84);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 85);

            for (; first != last; ++first)
                if (predication(*first))
                    break;

            return (first);
        }

        template<class TInputInterator, class TPredication> inline
        TInputInterator find_if(TInputInterator first, TInputInterator last, TPredication predication) {
            ::std::checked_assign_from_base(first, find_if(::std::checked_base(first), ::std::checked_base(last), predication));
            return (first);
        }

        template<class TForwardIterator> inline
        TForwardIterator adjacent_find(TForwardIterator first, TForwardIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 105);

            for (TForwardIterator firstB; (firstB = first) != last && ++first != last;)
                if (*firstB == *first)
                    return (firstB);

            return (last);
        }

        template<class TForwardIterator> inline
        TForwardIterator adjacent_find(TForwardIterator first, TForwardIterator last) {
            ::std::checked_assign_from_base(first, adjacent_find(::std::checked_base(first), ::std::checked_base(last)));
            return (first);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator adjacent_find(TForwardIterator first, TForwardIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 125);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 126);

            for (TForwardIterator firstB; (firstB = first) != last && ++first != last;)
                if (predication(*firstB, *first))
                    return (firstB);

            return (last);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator adjacent_find(TForwardIterator first, TForwardIterator last, TPredication predication) {
            ::std::checked_assign_from_base(first, adjacent_find(::std::checked_base(first), ::std::checked_base(last), predication));
            return (first);
        }

        template<class TInputInterator, class T> inline
        typename iterator_traits<TInputInterator>::difference_type
        count(TInputInterator first, TInputInterator last, const T& value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 148);
            typename iterator_traits<TInputInterator>::difference_type count = 0;

            for (; first != last; ++first)
                if (*first == value)
                    ++count;

            return (count);
        }

        template<class TInputInterator, class T> inline
        typename iterator_traits<TInputInterator>::difference_type
        count(TInputInterator first, TInputInterator last, const T& value) {
            return count(::std::checked_base(first), ::std::checked_base(last), value);
        }

        template<class TInputInterator, class TPredication> inline
        typename iterator_traits<TInputInterator>::difference_type
        count_if(TInputInterator first, TInputInterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 171);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 172);
            typename iterator_traits<TInputInterator>::difference_type count = 0;

            for (; first != last; ++first)
                if (predication(*first))
                    ++count;

            return (count);
        }

        template<class TInputInterator, class TPredication> inline
        typename iterator_traits<TInputInterator>::difference_type
        count_if(TInputInterator first, TInputInterator last, TPredication predication) {
            return count_if(::std::checked_base(first), ::std::checked_base(last), predication);
        }

        template<class TForwardIterator1, class TForwardIterator2, class TDifference1, class TDifference2> inline
        TForwardIterator1 search(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TDifference1*, TDifference2*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 197);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 198);
            TDifference1 count1 = 0;
            distance(first1, last1, count1);
            TDifference2 count2 = 0;
            distance(first2, last2, count2);

            for (; count2 <= count1; ++first1, --count1) {
                TForwardIterator1 middle1 = first1;

                for (TForwardIterator2 middle2 = first2; ; ++middle1, ++middle2)
                    if (middle2 == last2)
                        return (first1);
                    else if (!(*middle1 == *middle2))
                        break;
            }

            return (last1);
        }

        template<class TForwardIterator1, class TForwardIterator2> inline
        TForwardIterator1 search(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2) {
            ::std::checked_assign_from_base(first1, search(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), DistanceType(first1), DistanceType(first2)));
            return first1;
        }

        template<class TForwardIterator1, class TForwardIterator2, class TDifference1, class TDifference2, class TPredication> inline
        TForwardIterator1 search(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication, TDifference1*, TDifference2*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 237);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 238);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 239);
            TDifference1 count1 = 0;
            distance(first1, last1, count1);
            TDifference2 count2 = 0;
            distance(first2, last2, count2);

            for (; count2 <= count1; ++first1, --count1) {
                TForwardIterator1 middle1 = first1;

                for (TForwardIterator2 middle2 = first2; ; ++middle1, ++middle2)
                    if (middle2 == last2)
                        return (first1);
                    else if (!predication(*middle1, *middle2))
                        break;
            }

            return (last1);
        }

        template<class TForwardIterator1, class TForwardIterator2, class TPredication> inline
        TForwardIterator1 search(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication) {
            ::std::checked_assign_from_base(first1, search(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), predication, DistanceType(first1), DistanceType(first2)));
            return first1;
        }

        template<class TForwardIterator1, class TDifference2, class T> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value, forward_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 277);

            if (count <= 0)
                return (first1);

            for (; first1 != last1; ++first1)
                if (*first1 == value) {
                    TForwardIterator1 middle1  = first1;

                    for (TDifference2 count1 = count; ;)
                        if (--count1 == 0)
                            return (first1);
                        else if (++middle1 == last1)
                            return (last1);
                        else if (!(*middle1 == value))
                            break;

                    first1 = middle1;
                }

            return (last1);
        }

        template<class TForwardIterator1, class TDifference2, class T> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value, random_access_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 306);

            if (count <= 0)
                return (first1);

            TForwardIterator1 oldFirst1 = first1;

            for (; count <= last1 - oldFirst1;) {
                if (*first1 == value) {
                    TDifference2 count1 = count;
                    TForwardIterator1 middle1  = first1;

                    for (; oldFirst1 != first1 && first1[-1] == value; --first1)
                        --count1;

                    if (count1 <= last1 - middle1)
                        for (; ;)
                            if (--count1 == 0)
                                return (first1);
                            else if (!(*++middle1 == value))
                                break;

                    oldFirst1 = ++middle1;
                    first1 = oldFirst1;
                } else {
                    oldFirst1 = first1 + 1;
                    first1 += count;
                }
            }

            return (last1);
        }

        template<class TForwardIterator1, class TDifference2, class T> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value) {
            ::std::checked_assign_from_base(first1, search_n(::std::checked_base(first1), ::std::checked_base(last1), count, value, iterator_category(first1)));
            return first1;
        }

        template<class TForwardIterator1, class TDifference2, class T, class TPredication> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value, TPredication predication, forward_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 361);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 362);

            if (count <= 0)
                return (first1);

            for (; first1 != last1; ++first1)
                if (predication(*first1, value)) {
                    TForwardIterator1 middle1  = first1;

                    for (TDifference2 count1 = count; ;)
                        if (--count1 == 0)
                            return (first1);
                        else if (++middle1 == last1)
                            return (last1);
                        else if (!predication(*middle1, value))
                            break;

                    first1 = middle1;
                }

            return (last1);
        }

        template<class TForwardIterator1, class TDifference2, class T, class TPredication> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value, TPredication predication, random_access_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 392);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 393);

            if (count <= 0)
                return (first1);

            TForwardIterator1 oldFirst1 = first1;

            for (; count <= last1 - oldFirst1;) {
                if (predication(*first1, value)) {
                    TDifference2 count1 = count;
                    TForwardIterator1 middle1  = first1;

                    for (; oldFirst1 != first1 && first1[-1] == value; --first1)
                        --count1;

                    if (count1 <= last1 - middle1)
                        for (; ;)
                            if (--count1 == 0)
                                return (first1);
                            else if (!predication(*++middle1, value))
                                break;

                    oldFirst1 = ++middle1;
                    first1 = oldFirst1;
                } else {
                    oldFirst1 = first1 + 1;
                    first1 += count;
                }
            }

            return (last1);
        }

        template<class TForwardIterator1, class TDifference2, class T, class TPredication> inline
        TForwardIterator1 search_n(TForwardIterator1 first1, TForwardIterator1 last1, TDifference2 count, const T& value, TPredication predication) {
            ::std::checked_assign_from_base(first1, search_n(::std::checked_base(first1), ::std::checked_base(last1), count, value, predication, iterator_category(first1)));
            return first1;
        }

        template<class TForwardIterator1, class TForwardIterator2, class TDifference1, class TDifference2> inline
        TForwardIterator1 find_end(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TDifference1*, TDifference2*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 449);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 450);
            TDifference1 count1 = 0;
            distance(first1, last1, count1);
            TDifference2 count2 = 0;
            distance(first2, last2, count2);
            TForwardIterator1 ans = last1;

            if (0 < count2)
                for (; count2 <= count1; ++first1, --count1) {
                    TForwardIterator1 middle1 = first1;

                    for (TForwardIterator2 middle2 = first2; ; ++middle1)
                        if (!(*middle1 == *middle2))
                            break;
                        else if (++middle2 == last2) {
                            ans = first1;
                            break;
                        }
                }

            return (ans);
        }

        template<class TForwardIterator1, class TForwardIterator2> inline
        TForwardIterator1 find_end(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2) {
            ::std::checked_assign_from_base(first1, find_end(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), DistanceType(first1), DistanceType(first2)));
            return first1;
        }

        template<class TForwardIterator1, class TForwardIterator2, class TDifference1, class TDifference2, class TPredication> inline
        TForwardIterator1 find_end(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication, TDifference1*, TDifference2*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 494);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 495);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 496);
            TDifference1 count1 = 0;
            distance(first1, last1, count1);
            TDifference2 count2 = 0;
            distance(first2, last2, count2);
            TForwardIterator1 ans = last1;

            if (0 < count2)
                for (; count2 <= count1; ++first1, --count1) {
                    TForwardIterator1 middle1 = first1;

                    for (TForwardIterator2 middle2 = first2; ; ++middle1)
                        if (!predication(*middle1, *middle2))
                            break;
                        else if (++middle2 == last2) {
                            ans = first1;
                            break;
                        }
                }

            return (ans);
        }

        template<class TForwardIterator1, class TForwardIterator2, class TPredication> inline
        TForwardIterator1 find_end(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication) {
            ::std::checked_assign_from_base(first1, find_end(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), predication, DistanceType(first1), DistanceType(first2)));
            return first1;
        }

        template<class TForwardIterator1, class TForwardIterator2> inline
        TForwardIterator1 find_first_of(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 538);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 539);

            for (; first1 != last1; ++first1)
                for (TForwardIterator2 middle2 = first2; middle2 != last2; ++middle2)
                    if (*first1 == *middle2)
                        return (first1);

            return (first1);
        }

        template<class TForwardIterator1, class TForwardIterator2> inline
        TForwardIterator1 find_first_of(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 552);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 553);
            ::std::checked_assign_from_base(first1, find_first_of(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2)));
            return first1;
        }

        template<class TForwardIterator1, class TForwardIterator2, class TPredication> inline
        TForwardIterator1 find_first_of(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication) {
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 567);

            for (; first1 != last1; ++first1)
                for (TForwardIterator2 middle2 = first2; middle2 != last2; ++middle2)
                    if (predication(*first1, *middle2))
                        return (first1);

            return (first1);
        }

        template<class TForwardIterator1, class TForwardIterator2, class TPredication> inline
        TForwardIterator1 find_first_of(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, TForwardIterator2 last2, TPredication predication) {
            ::std::checked_assign_from_base(first1, find_first_of(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), predication));
            return (first1);
        }

        template<class TForwardIterator1, class TForwardIterator2> inline
        void iter_swap(TForwardIterator1 left, TForwardIterator2 right) {
            std::swap(*left, *right);
        }

        template<class TForwardIterator1, class TForwardIterator2, class _FwdItCats>
        inline
        TForwardIterator2 swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, _FwdItCats, range_checked_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 601);

            for (; first1 != last1; ++first1, ++first2)
                std::iter_swap(first1, first2);

            return (first2);
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        TForwardIterator2 swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2, random_access_iterator_tag, range_checked_iterator_tag) {
            TForwardIterator2 result = first2 + (last1 - first1);
            swap_ranges(first1, last1, ::std::checked_base(first2), forward_iterator_tag(), range_checked_iterator_tag());
            return (result);
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator2 >::result, TForwardIterator2 >::result
        swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2) {
            return swap_ranges(::std::checked_base(first1), ::std::checked_base(last1), first2,
                               iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator1, class TForwardElement2, size_t size>
        inline
        TForwardElement2* swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardElement2(&first2)[size]) {
            return (swap_ranges(first1, last1, ::stdext::make_checked_array_iterator(first2, size)).base());
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator2 >::result, TForwardIterator2 >::result
        swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2) {
            return swap_ranges(::std::checked_base(first1), ::std::checked_base(last1), first2,
                               iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TFunction1, class TIteratorCategory>
        inline
        TOutputInterator transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function, TIteratorCategory, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 665);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 666);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 667);

            for (; first != last; ++first, ++destination)
                *destination = function(*first);

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        TOutputInterator transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function, random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last - first);
            transform(first, last, ::std::checked_base(destination), function, forward_iterator_tag(), range_checked_iterator_tag());
            return (result);
        }

        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function) {
            return transform(::std::checked_base(first), ::std::checked_base(last), destination, function,
                             iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TFunction1, size_t size>
        inline
        TOutElement* transform(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TFunction1 function) {
            return (transform(first, last,
                              ::stdext::make_checked_array_iterator(destination, size), function).base());
        }

        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function) {
            return transform(::std::checked_base(first), ::std::checked_base(last), destination, function,
                             iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2, class TInputIteratorCategory, class TIteratorCategory>
        inline
        TOutputInterator transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                                   TOutputInterator destination, TFunction2 function,
                                   TInputIteratorCategory, TIteratorCategory,
                                   range_checked_iterator_tag, range_checked_iterator_tag) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 734);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 735);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 736);

            for (; first1 != last1; ++first1, ++first2, ++destination)
                *destination = function(*first1, *first2);

            return (destination);
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        TOutputInterator transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                                   TOutputInterator destination, TFunction2 function,
                                   random_access_iterator_tag, random_access_iterator_tag,
                                   range_checked_iterator_tag, range_checked_iterator_tag) {
            TInputIterator2 last2 = first2 + (last1 - first1);
            (last2);
            TOutputInterator result = destination + (last1 - first1);
            transform(first1, last1, ::std::checked_base(first2),
                      ::std::checked_base(destination), function,
                      forward_iterator_tag(), forward_iterator_tag(),
                      range_checked_iterator_tag(), range_checked_iterator_tag());
            return result;
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2, class TIteratorCategory>
        inline
        TOutputInterator transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                                   TOutputInterator destination, TFunction2 function,
                                   random_access_iterator_tag, TIteratorCategory,
                                   range_checked_iterator_tag, range_checked_iterator_tag) {
            TInputIterator2 last2 = first2 + (last1 - first1);
            (last2);
            return transform(first1, last1, ::std::checked_base(first2),
                             destination, function,
                             forward_iterator_tag(), forward_iterator_tag(),
                             range_checked_iterator_tag(), range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2, class TInputIteratorCategory>
        inline
        TOutputInterator transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                                   TOutputInterator destination, TFunction2 function,
                                   TInputIteratorCategory, random_access_iterator_tag,
                                   range_checked_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last1 - first1);
            transform(first1, last1, first2,
                      ::std::checked_base(destination), function,
                      forward_iterator_tag(), forward_iterator_tag(),
                      range_checked_iterator_tag(), range_checked_iterator_tag());
            return result;
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        typename ::std::enable_if < ::std::is_checked_iterator< TInputIterator2 >::result && ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                  TOutputInterator destination, TFunction2 function) {
            return transform(::std::checked_base(first1), ::std::checked_base(last1), first2, destination, function,
                             iterator_random(first1, first2), iterator_random(first1, destination),
                             ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInElement2, class TOutElement, class TFunction2, size_t SizeFirst2, size_t DestinationSize>
        inline
        TOutElement* transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2],
                               TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (transform(first1, last1,
                              ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                              ::stdext::make_checked_array_iterator(destination, DestinationSize),
                              function).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TFunction2, size_t DestinationSize>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, TOutElement* >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                  TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (transform(::std::checked_base(first1), ::std::checked_base(last1), first2,
                              ::stdext::make_checked_array_iterator(destination, DestinationSize), function,
                              iterator_random(first1, first2), iterator_category(first1),
                              ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TFunction2, size_t DestinationSize>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, TOutElement* >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                  TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (transform(::std::checked_base(first1), ::std::checked_base(last1), first2,
                              ::stdext::make_checked_array_iterator(destination, DestinationSize), function,
                              iterator_random(first1, first2), iterator_category(first1),
                              ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()).base());
        }

        template<class TInputIterator1, class TInElement2, class TOutputInterator, class TFunction2, size_t SizeFirst2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2],
                  TOutputInterator destination, TFunction2 function) {
            return (transform(::std::checked_base(first1), ::std::checked_base(last1),
                              ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                              destination, function,
                              iterator_category(first1), iterator_random(first1, destination),
                              ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()));
        }

        template<class TInputIterator1, class TInElement2, class TOutputInterator, class TFunction2, size_t SizeFirst2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2],
                  TOutputInterator destination, TFunction2 function) {
            return (transform(::std::checked_base(first1), ::std::checked_base(last1),
                              ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                              destination, function,
                              iterator_category(first1), iterator_random(first1, destination),
                              ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result || !::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                  TOutputInterator destination, TFunction2 function) {
            return transform(::std::checked_base(first1), ::std::checked_base(last1), first2, destination, function,
                             iterator_random(first1, first2), iterator_random(first1, destination),
                             ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class T> inline
        void replace(TForwardIterator first, TForwardIterator last, const T& oldValue, const T& newValue) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 894);

            for (; first != last; ++first)
                if (*first == oldValue)
                    *first = newValue;
        }

        template<class TForwardIterator, class T> inline
        void replace(TForwardIterator first, TForwardIterator last, const T& oldValue, const T& newValue) {
            replace(::std::checked_base(first), ::std::checked_base(last), oldValue, newValue);
        }

        template<class TForwardIterator, class TPredication, class T> inline
        void Replace_if(TForwardIterator first, TForwardIterator last, TPredication predication, const T& value) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 914);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 915);

            for (; first != last; ++first)
                if (predication(*first))
                    *first = value;
        }

        template<class TForwardIterator, class TPredication, class T> inline
        void replace_if(TForwardIterator first, TForwardIterator last, TPredication predication, const T& value) {
            Replace_if(::std::checked_base(first), ::std::checked_base(last), predication, value);
        }

        template<class TInputInterator, class TOutputInterator, class T, class TIteratorCategory>
        inline
        TOutputInterator replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                      const T& oldValue, const T& newValue,
                                      TIteratorCategory, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 936);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 937);

            for (; first != last; ++first, ++destination)
                *destination = *first == oldValue ? newValue : *first;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class T>
        inline
        TOutputInterator replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                      const T& oldValue, const T& newValue,
                                      random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last - first);
            replace_copy(first, last, ::std::checked_base(destination),
                         oldValue, newValue,
                         forward_iterator_tag(), range_checked_iterator_tag());
            return (result);
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                     const T& oldValue, const T& newValue) {
            return replace_copy(::std::checked_base(first), ::std::checked_base(last), destination, oldValue, newValue,
                                iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class T, size_t size>
        inline
        TOutElement* replace_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size],
                                  const T& oldValue, const T& newValue) {
            return (replace_copy(first, last,
                                 ::stdext::make_checked_array_iterator(destination, size),
                                 oldValue, newValue).base());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& oldValue, const T& newValue) {
            return replace_copy(::std::checked_base(first), ::std::checked_base(last), destination, oldValue, newValue,
                                iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T, class TIteratorCategory>
        inline
        TOutputInterator replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                         TPredication predication, const T& value, TIteratorCategory, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1012);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1013);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1014);

            for (; first != last; ++first, ++destination)
                *destination = predication(*first) ? value : *first;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T>
        inline
        TOutputInterator replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                         TPredication predication, const T& value,
                                         random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last - first);
            replace_copy_if(first, last, ::std::checked_base(destination),
                            predication, value,
                            forward_iterator_tag(), range_checked_iterator_tag());
            return (result);
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, const T& value) {
            return replace_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, value,
                                   iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TPredication, class T, size_t size>
        inline
        TOutElement* replace_copy_if(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication, const T& value) {
            return (replace_copy_if(first, last,
                                    ::stdext::make_checked_array_iterator(destination, size),
                                    predication, value).base());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, const T& value) {
            return replace_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, value,
                                   iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TFunction0> inline
        void generate(TForwardIterator first, TForwardIterator last, TFunction0 function) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1091);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 1092);

            for (; first != last; ++first)
                *first = function();
        }

        template<class TForwardIterator, class TFunction0> inline
        void generate(TForwardIterator first, TForwardIterator last, TFunction0 function) {
            generate(::std::checked_base(first), ::std::checked_base(last), function);
        }

        template<class TOutputInterator, class TDifference, class TFunction0, class _OutItCat>
        inline
        void generate_n(TOutputInterator destination, TDifference count, TFunction0 function, _OutItCat, range_checked_iterator_tag) {
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1110);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 1111);

            for (; 0 < count; --count, ++destination)
                *destination = function();
        }

        template<class TOutputInterator, class TDifference, class TFunction0>
        inline
        void generate_n(TOutputInterator destination, TDifference count, TFunction0 function,
                        random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + count;
            generate_n(::std::checked_base(destination), count, function,
                       forward_iterator_tag(), range_checked_iterator_tag());
        }

        template<class TOutputInterator, class TDifference, class TFunction0> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        generate_n(TOutputInterator destination, TDifference count, TFunction0 function) {
            generate_n(destination, count, function,
                       iterator_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TOutElement, class TDifference, class TFunction0, size_t size>
        inline
        void generate_n(TOutElement(&destination)[size], TDifference count, TFunction0 function) {
            generate_n(::stdext::make_checked_array_iterator(destination, size), count, function);
        }

        template<class TOutputInterator, class TDifference, class TFunction0> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        generate_n(TOutputInterator destination, TDifference count, TFunction0 function) {
            generate_n(destination, count, function, iterator_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        TOutputInterator remove_copy(TInputInterator first, TInputInterator last,
                                     TOutputInterator destination, const T& value, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1177);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1178);

            for (; first != last; ++first)
                if (!(*first == value))
                    *destination++ = *first;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        remove_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& value) {
            return remove_copy(::std::checked_base(first), ::std::checked_base(last), destination, value, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class T, size_t size>
        inline
        TOutElement* remove_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], const T& value) {
            return (remove_copy(first, last,
                                ::stdext::make_checked_array_iterator(destination, size),
                                value).base());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        remove_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& value) {
            return remove_copy(::std::checked_base(first), ::std::checked_base(last), destination, value, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        TOutputInterator remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1235);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1236);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1237);

            for (; first != last; ++first)
                if (!predication(*first))
                    *destination++ = *first;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return remove_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* remove_copy_if(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication) {
            return (remove_copy_if(first, last,
                                   ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return remove_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class T> inline
        TForwardIterator remove(TForwardIterator first, TForwardIterator last, const T& value) {
            first = find(first, last, value);

            if (first == last)
                return (first);
            else {
                TForwardIterator first1 = first;
                return (::stdext::unchecked_remove_copy(++first1, last, first, value));
            }
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator remove_if(TForwardIterator first, TForwardIterator last, TPredication predication) {
            first = std::find_if(first, last, predication);

            if (first == last)
                return (first);
            else {
                TForwardIterator first1 = first;
                return (::stdext::unchecked_remove_copy_if(++first1, last, first, predication));
            }
        }

        template<class TForwardIterator> inline
        TForwardIterator unique(TForwardIterator first, TForwardIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1317);

            for (TForwardIterator firstB; (firstB = first) != last && ++first != last;)
                if (*firstB == *first) {
                    for (; ++first != last;)
                        if (!(*firstB == *first))
                            *++firstB = *first;

                    return (++firstB);
                }

            return (last);
        }

        template<class TForwardIterator> inline
        TForwardIterator unique(TForwardIterator first, TForwardIterator last) {
            ::std::checked_assign_from_base(last, unique(::std::checked_base(first), ::std::checked_base(last)));
            return (last);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator unique(TForwardIterator first, TForwardIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1342);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1343);

            for (TForwardIterator firstB; (firstB = first) != last && ++first != last;)
                if (predication(*firstB, *first)) {
                    for (; ++first != last;)
                        if (!predication(*firstB, *first))
                            *++firstB = *first;

                    return (++firstB);
                }

            return (last);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator unique(TForwardIterator first, TForwardIterator last, TPredication predication) {
            ::std::checked_assign_from_base(last, unique(::std::checked_base(first), ::std::checked_base(last), predication));
            return (last);
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        TOutputInterator unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, T*, range_checked_iterator_tag) {
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1370);
            T value = *first;

            for (*destination++ = value; ++first != last;)
                if (!(value == *first))
                    value = *first, *destination++ = value;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator> inline
        TOutputInterator unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                     input_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, ValueType(first), range_checked_iterator_tag()));
        }

        template<class TForwardIterator, class TOutputInterator> inline
        TOutputInterator unique_copy(TForwardIterator first, TForwardIterator last, TOutputInterator destination,
                                     forward_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1392);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1393);
            TForwardIterator firstB = first;

            for (*destination++ = *firstB; ++first != last;)
                if (!(*firstB == *first))
                    firstB = first, *destination++ = *firstB;

            return (destination);
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        TOutputInterator unique_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                                     bidirectional_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, forward_iterator_tag(), range_checked_iterator_tag()));
        }

        template<class TRandomIterator, class TOutputInterator> inline
        TOutputInterator unique_copy(TRandomIterator first, TRandomIterator last, TOutputInterator destination,
                                     random_access_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, forward_iterator_tag(), range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (first == last ? destination :
                    unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutElement, size_t size>
        inline
        TOutElement* unique_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
            return (first == last ? destination :
                    (unique_copy(first, last, ::stdext::make_checked_array_iterator(destination, size)).base()));
        }

        template<class TInputInterator, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (first == last ? destination :
                    unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator, class T, class TPredication> inline
        TOutputInterator unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication,
                                     T*, range_checked_iterator_tag) {
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1464);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1465);
            T value = *first;

            for (*destination++ = value; ++first != last;)
                if (!predication(value, *first))
                    value = *first, *destination++ = value;

            return (destination);
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication,
                                     input_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, predication, ValueType(first), range_checked_iterator_tag()));
        }

        template<class TForwardIterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unique_copy(TForwardIterator first, TForwardIterator last, TOutputInterator destination, TPredication predication,
                                     forward_iterator_tag, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1489);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1490);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1491);
            TForwardIterator firstB = first;

            for (*destination++ = *firstB; ++first != last;)
                if (!predication(*firstB, *first))
                    firstB = first, *destination++ = *firstB;

            return (destination);
        }

        template<class TBidirectionIterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unique_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TPredication predication,
                                     bidirectional_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, predication,
                                forward_iterator_tag(), range_checked_iterator_tag()));
        }

        template<class TRandomIterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unique_copy(TRandomIterator first, TRandomIterator last, TOutputInterator destination, TPredication predication,
                                     random_access_iterator_tag, range_checked_iterator_tag) {
            return (unique_copy(first, last, destination, predication,
                                forward_iterator_tag(), range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return (first == last ? destination
                    : unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, predication, iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* unique_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication) {
            return (first == last ? destination
                    : (unique_copy(first, last, ::stdext::make_checked_array_iterator(destination, size), predication).base()));
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return (first == last ? destination
                    : unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, predication, iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TBidirectionIterator> inline
        void reverse(TBidirectionIterator first, TBidirectionIterator last, bidirectional_iterator_tag) {
            for (; first != last && first != --last; ++first)
                std::iter_swap(first, last);
        }

        template<class TRandomIterator> inline
        void reverse(TRandomIterator first, TRandomIterator last, random_access_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1573);

            for (; first < last; ++first)
                std::iter_swap(first, --last);
        }

        template<class TBidirectionIterator> inline
        void reverse(TBidirectionIterator first, TBidirectionIterator last) {
            reverse(::std::checked_base(first), ::std::checked_base(last), iterator_category(first));
        }

        template<class TBidirectionIterator, class TOutputInterator, class TIteratorCategory>
        inline
        TOutputInterator reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TIteratorCategory, range_checked_iterator_tag) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1590);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 1591);

            for (; first != last; ++destination)
                *destination = *--last;

            return (destination);
        }

        template<class TBidirectionIterator, class TOutputInterator>
        inline
        TOutputInterator reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                                      random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last - first);
            reverse_copy(first, last, ::std::checked_base(destination),
                         forward_iterator_tag(), range_checked_iterator_tag());
            return (result);
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination) {
            return reverse_copy(::std::checked_base(first), ::std::checked_base(last), destination, iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutElement, size_t size>
        inline
        TOutElement* reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size]) {
            return (reverse_copy(first, last, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination) {
            return reverse_copy(::std::checked_base(first), ::std::checked_base(last), destination, iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator> inline
        void rotate(TForwardIterator first, TForwardIterator middle, TForwardIterator last, forward_iterator_tag) {
            for (TForwardIterator next = middle; ;) {
                std::iter_swap(first, next);

                if (++first == middle)
                    if (++next == last)
                        break;
                    else
                        middle = next;
                else if (++next == last)
                    next = middle;
            }
        }

        template<class TBidirectionIterator> inline
        void rotate(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last, bidirectional_iterator_tag) {
            std::reverse(first, middle);
            std::reverse(middle, last);
            std::reverse(first, last);
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void rotate(TRandomIterator first, TRandomIterator middle, TRandomIterator last, TDifference*, T*) {
            debug_range(first, middle, L"..\\ce\\include\\algorithm", 1678);
            debug_range(middle, last, L"..\\ce\\include\\algorithm", 1679);
            TDifference shift = middle - first;
            TDifference count = last - first;

            for (TDifference factor = shift; factor != 0;) {
                TDifference temp = count % factor;
                count = factor, factor = temp;
            }

            if (count < last - first)
                for (; 0 < count; --count) {
                    TRandomIterator hole = first + count;
                    TRandomIterator next = hole;
                    T holeValue = *hole;
                    TRandomIterator next1 = next + shift == last ? first : next + shift;

                    while (next1 != hole) {
                        *next = *next1;
                        next = next1;
                        next1 = shift < last - next1 ? next1 + shift
                                : first + (shift - (last - next1));
                    }

                    *next = holeValue;
                }
        }

        template<class TRandomIterator> inline
        void rotate(TRandomIterator first, TRandomIterator middle, TRandomIterator last, random_access_iterator_tag) {
            rotate(first, middle, last, DistanceType(first), ValueType(first));
        }

        template<class TForwardIterator> inline
        void rotate(TForwardIterator first, TForwardIterator middle, TForwardIterator last) {
            if (first != middle && middle != last)
                rotate(::std::checked_base(first), ::std::checked_base(middle), ::std::checked_base(last), iterator_category(first));
        }

        template<class TForwardIterator, class TOutputInterator> inline
        TOutputInterator rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination, range_checked_iterator_tag) {
            destination = ::stdext::unchecked_copy(middle, last, destination);
            return (::stdext::unchecked_copy(first, middle, destination));
        }

        template<class TForwardIterator, class TOutputInterator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination) {
            return rotate_copy(first, middle, last, destination, ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TOutElement, size_t size>
        inline
        TOutElement* rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutElement(&destination)[size]) {
            return (rotate_copy(first, middle, last, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TForwardIterator, class TOutputInterator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination) {
            return rotate_copy(first, middle, last, destination, ::std::range_checked_iterator_tag());
        }

        template<class TRandomIterator, class TDifference> inline
        void random_shuffle(TRandomIterator first, TRandomIterator last, TDifference*) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1770);
            const int RANDOM_BITS = 15;
            const int RANDOM_MAX = (1U << RANDOM_BITS) - 1;
            TRandomIterator next = first;

            for (unsigned long index = 2; ++next != last; ++index) {
                unsigned long randomMax = RANDOM_MAX;
                unsigned long random = ::rand() & RANDOM_MAX;

                for (; randomMax < index && randomMax != ~0UL;
                        randomMax = randomMax << RANDOM_BITS | RANDOM_MAX)
                    random = random << RANDOM_BITS
                             | (::rand() & RANDOM_MAX);

                std::iter_swap(next, first + TDifference(random % index));
            }
        }

        template<class TRandomIterator> inline
        void random_shuffle(TRandomIterator first, TRandomIterator last) {
            if (first != last)
                random_shuffle(::std::checked_base(first), ::std::checked_base(last), DistanceType(first));
        }

        template<class TRandomIterator, class TFunction1, class TDifference> inline
        void random_shuffle(TRandomIterator first, TRandomIterator last, TFunction1& function, TDifference*) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1801);
            debug_pointer(function, L"..\\ce\\include\\algorithm", 1802);
            TRandomIterator next = first;

            for (TDifference index = 2; ++next != last; ++index)
                std::iter_swap(next, first + TDifference(function(index) % index));
        }

        template<class TRandomIterator, class TFunction1> inline
        void random_shuffle(TRandomIterator first, TRandomIterator last, TFunction1& function) {
            if (first != last)
                random_shuffle(::std::checked_base(first), ::std::checked_base(last), function, DistanceType(first));
        }

        template<class TBidirectionIterator, class TPredication> inline
        TBidirectionIterator partition(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1822);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 1823);

            for (; ; ++first) {
                for (; first != last && predication(*first); ++first)
                    ;

                if (first == last)
                    break;

                for (; first != --last && !predication(*last);)
                    ;

                if (first == last)
                    break;

                std::iter_swap(first, last);
            }

            return (first);
        }

        template<class TBidirectionIterator, class TPredication> inline
        TBidirectionIterator partition(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            ::std::checked_assign_from_base(first, partition(::std::checked_base(first), ::std::checked_base(last), predication));
            return (first);
        }

        template<class TBidirectionIterator, class TPredication, class TDifference, class T> inline
        TBidirectionIterator stable_partition(TBidirectionIterator first, TBidirectionIterator last, TPredication predication, TDifference count, TempIterator<T>& tempBuffer) {
            if (count == 0)
                return (first);
            else if (count == 1)
                return (predication(*first) ? last : first);
            else if (count <= tempBuffer.maxLength()) {
                TBidirectionIterator next = first;

                for (tempBuffer._Init(); first != last; ++first)
                    if (predication(*first))
                        *next++ = *first;
                    else
                        *tempBuffer++ = *first;

                ::stdext::unchecked_copy(tempBuffer.first(), tempBuffer.last(), next);
                return (next);
            } else {
                TBidirectionIterator middle = first;
                std::advance(middle, count / 2);
                TBidirectionIterator left = stable_partition(first, middle, predication,
                                            count / 2, tempBuffer);
                TBidirectionIterator right = stable_partition(middle, last, predication,
                                             count - count / 2, tempBuffer);
                TDifference count1 = 0;
                distance(left, middle, count1);
                TDifference count2 = 0;
                distance(middle, right, count2);
                return (buffered_rotate(left, middle, right, count1, count2, tempBuffer));
            }
        }

        template<class TBidirectionIterator, class TPredication, class TDifference, class T> inline
        TBidirectionIterator stable_partition(TBidirectionIterator first, TBidirectionIterator last, TPredication predication, TDifference*, T*) {
            TDifference count = 0;
            distance(first, last, count);
            TempIterator<T> tempBuffer(count);
            return (stable_partition(first, last, predication, count, tempBuffer));
        }

        template<class TBidirectionIterator, class TPredication> inline
        TBidirectionIterator stable_partition(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            if (first != last) {
                ::std::checked_assign_from_base(first, stable_partition(::std::checked_base(first), ::std::checked_base(last), predication, DistanceType(first), ValueType(first)));
            }

            return first;
        }

        template<class TRandomIterator> inline
        void debug_heap(TRandomIterator first, TRandomIterator last) {
            if (first != last)
                for (TRandomIterator _Root = first; ++first != last; ++_Root)
                    if (debug_lt(*_Root, *first, L"..\\ce\\include\\algorithm", 1931))
                        debug_message(L"invalid heap", L"..\\ce\\include\\algorithm", 1932);
                    else if (++first == last)
                        break;
                    else if (debug_lt(*_Root, *first, L"..\\ce\\include\\algorithm", 1935))
                        debug_message(L"invalid heap", L"..\\ce\\include\\algorithm", 1936);
        }

        template<class TRandomIterator, class TPredication> inline
        void debug_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            if (first != last)
                for (TRandomIterator _Root = first; ++first != last; ++_Root)
                    if (debug_lt_pred(predication, *_Root, *first, L"..\\ce\\include\\algorithm", 1946))
                        debug_message(L"invalid heap", L"..\\ce\\include\\algorithm", 1947);
                    else if (++first == last)
                        break;
                    else if (debug_lt_pred(predication, *_Root, *first, L"..\\ce\\include\\algorithm", 1950))
                        debug_message(L"invalid heap", L"..\\ce\\include\\algorithm", 1951);
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void push_heap(TRandomIterator first, TDifference hole, TDifference top, T value) {
            for (TDifference index = (hole - 1) / 2;
                    top < hole && debug_lt(*(first + index), value, L"..\\ce\\include\\algorithm", 1972);
                    index = (hole - 1) / 2) {
                *(first + hole) = *(first + index);
                hole = index;
            }

            *(first + hole) = value;
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void push_heap_0(TRandomIterator first, TRandomIterator last, TDifference*, T*) {
            TDifference count = last - first;

            if (0 < count)
                std::push_heap(first, count, TDifference(0), T(*last));
        }

        template<class TRandomIterator> inline
        void push_heap(TRandomIterator first, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 1995);

            if (first != last) {
                debug_heap(first, last - 1);
                std::push_heap_0(::std::checked_base(first), ::std::checked_base(--last),
                                 DistanceType(first), ValueType(first));
            }
        }

        template<class TRandomIterator, class TDifference, class T, class TPredication> inline
        void push_heap(TRandomIterator first, TDifference hole, TDifference top, T value, TPredication predication) {
            for (TDifference index = (hole - 1) / 2;
                    top < hole && debug_lt_pred(predication, *(first + index), value, L"..\\ce\\include\\algorithm", 2013);
                    index = (hole - 1) / 2) {
                *(first + hole) = *(first + index);
                hole = index;
            }

            *(first + hole) = value;
        }

        template<class TRandomIterator, class TDifference, class T, class TPredication> inline
        void push_heap_0(TRandomIterator first, TRandomIterator last, TPredication predication, TDifference*, T*) {
            TDifference count = last - first;

            if (0 < count)
                std::push_heap(first, count, TDifference(0), T(*last), predication);
        }

        template<class TRandomIterator, class TPredication> inline
        void push_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2038);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2039);

            if (first != last) {
                debug_heap(first, last - 1, predication);
                std::push_heap_0(::std::checked_base(first), ::std::checked_base(--last), predication, DistanceType(first), ValueType(first));
            }
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void adjust_heap(TRandomIterator first, TDifference hole, TDifference bottom, T value) {
            TDifference top = hole;
            TDifference index = 2 * hole + 2;

            for (; index < bottom; index = 2 * index + 2) {
                if (debug_lt(*(first + index), *(first + (index - 1)), L"..\\ce\\include\\algorithm", 2059))
                    --index;

                *(first + hole) = *(first + index), hole = index;
            }

            if (index == bottom) {
                *(first + hole) = *(first + (bottom - 1));
                hole = bottom - 1;
            }

            std::push_heap(first, hole, top, value);
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void pop_heap(TRandomIterator first, TRandomIterator last, TRandomIterator destination, T value, TDifference*) {
            *destination = *first;
            std::adjust_heap(first, TDifference(0), TDifference(last - first), value);
        }

        template<class TRandomIterator, class T> inline
        void pop_heap_0(TRandomIterator first, TRandomIterator last, T*) {
            pop_heap(first, last - 1, last - 1,
                     T(*(last - 1)), DistanceType(first));
        }

        template<class TRandomIterator> inline
        void pop_heap(TRandomIterator first, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2093);
            debug_heap(first, last);

            if (1 < last - first)
                pop_heap_0(::std::checked_base(first), ::std::checked_base(last), ValueType(first));
        }

        template<class TRandomIterator, class TDifference, class T, class TPredication> inline
        void adjust_heap(TRandomIterator first, TDifference hole, TDifference bottom, T value, TPredication predication) {
            TDifference top = hole;
            TDifference index = 2 * hole + 2;

            for (; index < bottom; index = 2 * index + 2) {
                if (debug_lt_pred(predication, *(first + index), *(first + (index - 1)), L"..\\ce\\include\\algorithm", 2112))
                    --index;

                *(first + hole) = *(first + index), hole = index;
            }

            if (index == bottom) {
                *(first + hole) = *(first + (bottom - 1));
                hole = bottom - 1;
            }

            std::push_heap(first, hole, top, value, predication);
        }

        template<class TRandomIterator, class TDifference, class T, class TPredication> inline
        void pop_heap(TRandomIterator first, TRandomIterator last, TRandomIterator destination, T value, TPredication predication, TDifference*) {
            *destination = *first;
            std::adjust_heap(first, TDifference(0), TDifference(last - first), value, predication);
        }

        template<class TRandomIterator, class T, class TPredication> inline
        void pop_heap_0(TRandomIterator first, TRandomIterator last, TPredication predication, T*) {
            pop_heap(first, last - 1, last - 1, T(*(last - 1)), predication, DistanceType(first));
        }

        template<class TRandomIterator, class TPredication> inline
        void pop_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2150);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2151);
            debug_heap(first, last, predication);

            if (1 < last - first)
                pop_heap_0(::std::checked_base(first), ::std::checked_base(last), predication, ValueType(first));
        }

        template<class TRandomIterator, class TDifference, class T> inline
        void make_heap(TRandomIterator first, TRandomIterator last, TDifference*, T*) {
            TDifference bottom = last - first;

            for (TDifference hole = bottom / 2; 0 < hole;) {
                --hole;
                std::adjust_heap(first, hole, bottom,
                                 T(*(first + hole)));
            }
        }

        template<class TRandomIterator> inline
        void make_heap(TRandomIterator first, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2176);

            if (1 < last - first)
                make_heap(::std::checked_base(first), ::std::checked_base(last),
                          DistanceType(first), ValueType(first));
        }

        template<class TRandomIterator, class TDifference, class T, class TPredication> inline
        void make_heap(TRandomIterator first, TRandomIterator last, TPredication predication, TDifference*, T*) {
            TDifference bottom = last - first;

            for (TDifference hole = bottom / 2; 0 < hole;) {
                --hole;
                std::adjust_heap(first, hole, bottom,
                                 T(*(first + hole)), predication);
            }
        }

        template<class TRandomIterator, class TPredication> inline
        void make_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2202);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2203);

            if (1 < last - first)
                make_heap(::std::checked_base(first), ::std::checked_base(last), predication,
                          DistanceType(first), ValueType(first));
        }

        template<class TRandomIterator> inline
        void sort_heap(TRandomIterator first, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2213);
            debug_heap(first, last);

            for (; 1 < last - first; --last)
                std::pop_heap(first, last);
        }

        template<class TRandomIterator> inline
        void sort_heap(TRandomIterator first, TRandomIterator last) {
            debug_heap(first, last);
            sort_heap(::std::checked_base(first), ::std::checked_base(last));
        }

        template < class TRandomIterator,
                 class TPredication > inline
        void sort_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            for (; 1 < last - first; --last)
                std::pop_heap(first, last, predication);
        }

        template<class TRandomIterator, class TPredication> inline
        void sort_heap(TRandomIterator first, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 2239);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2240);
            debug_heap(first, last, predication);
            sort_heap(::std::checked_base(first), ::std::checked_base(last), predication);
        }

        template<class TForwardIterator, class T, class TDifference> inline
        TForwardIterator lower_bound(TForwardIterator first, TForwardIterator last, const T& value, TDifference*) {
            debug_order_single(first, last, true, L"..\\ce\\include\\algorithm", 2251);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, false, L"..\\ce\\include\\algorithm", 2260);

                if (debug_lt(*middle, value, L"..\\ce\\include\\algorithm", 2262))
                    first = ++middle, count -= count2 + 1;
                else
                    count = count2;
            }

            return (first);
        }

        template<class TForwardIterator, class T> inline
        TForwardIterator lower_bound(TForwardIterator first, TForwardIterator last, const T& value) {
            ::std::checked_assign_from_base(first, lower_bound(::std::checked_base(first), ::std::checked_base(last), value, DistanceType(first)));
            return first;
        }

        template<class TForwardIterator, class T, class TDifference, class TPredication> inline
        TForwardIterator lower_bound(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication, TDifference*) {
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2287);
            debug_order_single(first, last, predication, true, L"..\\ce\\include\\algorithm", 2288);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, predication, false, L"..\\ce\\include\\algorithm", 2296);

                if (debug_lt_pred(predication, *middle, value, L"..\\ce\\include\\algorithm", 2298))
                    first = ++middle, count -= count2 + 1;
                else
                    count = count2;
            }

            return (first);
        }

        template<class TForwardIterator, class T, class TPredication> inline
        TForwardIterator lower_bound(TForwardIterator first, TForwardIterator last,
                                     const T& value, TPredication predication) {
            ::std::checked_assign_from_base(first, lower_bound(::std::checked_base(first), ::std::checked_base(last), value, predication, DistanceType(first)));
            return first;
        }

        template<class TForwardIterator, class T, class TDifference> inline
        TForwardIterator upper_bound(TForwardIterator first, TForwardIterator last, const T& value, TDifference*) {
            debug_order_single(first, last, true, L"..\\ce\\include\\algorithm", 2323);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, false, L"..\\ce\\include\\algorithm", 2331);

                if (!debug_lt(value, *middle, L"..\\ce\\include\\algorithm", 2333))
                    first = ++middle, count -= count2 + 1;
                else
                    count = count2;
            }

            return (first);
        }

        template<class TForwardIterator, class T> inline
        TForwardIterator upper_bound(TForwardIterator first, TForwardIterator last, const T& value) {
            ::std::checked_assign_from_base(first, upper_bound(::std::checked_base(first), ::std::checked_base(last), value, DistanceType(first)));
            return first;
        }

        template<class TForwardIterator, class T, class TDifference, class TPredication> inline
        TForwardIterator upper_bound(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication, TDifference*) {
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2358);
            debug_order_single(first, last, predication, true, L"..\\ce\\include\\algorithm", 2359);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, predication, false, L"..\\ce\\include\\algorithm", 2367);

                if (!debug_lt_pred(predication, value, *middle, L"..\\ce\\include\\algorithm", 2369))
                    first = ++middle, count -= count2 + 1;
                else
                    count = count2;
            }

            return (first);
        }

        template<class TForwardIterator, class T, class TPredication> inline
        TForwardIterator upper_bound(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication) {
            ::std::checked_assign_from_base(first, upper_bound(::std::checked_base(first), ::std::checked_base(last), value, predication, DistanceType(first)));
            return first;
        }

        template<class TForwardIterator, class T, class TDifference> inline
        pair<TForwardIterator, TForwardIterator> equal_range(TForwardIterator first, TForwardIterator last, const T& value, TDifference*) {
            debug_order_single(first, last, true, L"..\\ce\\include\\algorithm", 2395);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, false, L"..\\ce\\include\\algorithm", 2404);

                if (debug_lt(*middle, value, L"..\\ce\\include\\algorithm", 2406)) {
                    first = ++middle;
                    count -= count2 + 1;
                } else if (value < *middle)
                    count = count2;
                else {
                    TForwardIterator first2 = lower_bound(first, middle, value);
                    std::advance(first, count);
                    TForwardIterator last2 = upper_bound(++middle, first, value);
                    return (pair<TForwardIterator, TForwardIterator>(first2, last2));
                }
            }

            return (pair<TForwardIterator, TForwardIterator>(first, first));
        }

        template<class TForwardIterator, class T> inline
        pair<TForwardIterator, TForwardIterator> equal_range(TForwardIterator first, TForwardIterator last, const T& value) {
            return (equal_range(first, last, value, DistanceType(first)));
        }

        template<class TForwardIterator, class T, class TDifference, class TPredication> inline
        pair<TForwardIterator, TForwardIterator>
        equal_range(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication, TDifference*) {
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 2441);
            debug_order_single(first, last, predication, true, L"..\\ce\\include\\algorithm", 2442);
            TDifference count = 0;
            distance(first, last, count);

            for (; 0 < count;) {
                TDifference count2 = count / 2;
                TForwardIterator middle = first;
                std::advance(middle, count2);
                debug_order_single(middle, last, predication, false, L"..\\ce\\include\\algorithm", 2451);

                if (debug_lt_pred(predication, *middle, value, L"..\\ce\\include\\algorithm", 2453)) {
                    first = ++middle;
                    count -= count2 + 1;
                } else if (predication(value, *middle))
                    count = count2;
                else {
                    TForwardIterator first2 = lower_bound(first, middle, value, predication);
                    std::advance(first, count);
                    TForwardIterator last2 = upper_bound(++middle, first, value, predication);
                    return (pair<TForwardIterator, TForwardIterator>(first2, last2));
                }
            }

            return (pair<TForwardIterator, TForwardIterator>(first, first));
        }

        template<class TForwardIterator, class T, class TPredication> inline
        pair<TForwardIterator, TForwardIterator> equal_range(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication) {
            return (equal_range(first, last, value, predication, DistanceType(first)));
        }

        template<class TForwardIterator, class T> inline
        bool binary_search(TForwardIterator first, TForwardIterator last, const T& value) {
            first = std::lower_bound(first, last, value);
            return (first != last && !(value < *first));
        }

        template<class TForwardIterator, class T, class TPredication> inline
        bool binary_search(TForwardIterator first, TForwardIterator last, const T& value, TPredication predication) {
            first = std::lower_bound(first, last, value, predication);
            return (first != last && !predication(value, *first));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TIteratorCategory>
        inline
        TOutputInterator merge(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination,
                               TIteratorCategory, range_checked_iterator_tag) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 2508);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 2509);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 2510);

            for (; first1 != last1 && first2 != last2; ++destination)
                if (debug_lt(*first2, *first1, L"..\\ce\\include\\algorithm", 2512))
                    *destination = *first2, ++first2;
                else
                    *destination = *first1, ++first1;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator>
        inline
        TOutputInterator merge(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination,
                               random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last1 - first1) + (last2 - first2);
            merge(first1, last1, first2, last2, ::std::checked_base(destination),
                  forward_iterator_tag(), range_checked_iterator_tag());
            return result;
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return merge(::std::checked_base(first1), ::std::checked_base(last1),
                         ::std::checked_base(first2), ::std::checked_base(last2), destination,
                         iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (merge(first1, last1, first2, last2, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return merge(::std::checked_base(first1), ::std::checked_base(last1),
                         ::std::checked_base(first2), ::std::checked_base(last2), destination,
                         iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication, class TIteratorCategory>
        inline
        TOutputInterator merge(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication,
                               TIteratorCategory, range_checked_iterator_tag) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 2592);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 2593);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 2594);

            for (; first1 != last1 && first2 != last2; ++destination)
                if (debug_lt_pred(predication, *first2, *first1, L"..\\ce\\include\\algorithm", 2596))
                    *destination = *first2, ++first2;
                else
                    *destination = *first1, ++first1;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication>
        inline
        TOutputInterator merge(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication,
                               random_access_iterator_tag, range_checked_iterator_tag) {
            TOutputInterator result = destination + (last1 - first1) + (last2 - first2);
            merge(first1, last1, first2, last2, ::std::checked_base(destination), predication,
                  forward_iterator_tag(), range_checked_iterator_tag());
            return result;
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return merge(::std::checked_base(first1), ::std::checked_base(last1),
                         ::std::checked_base(first2), ::std::checked_base(last2),
                         destination, predication,
                         iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* merge(TInputIterator1 first1, TInputIterator1 last1,
                           TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (merge(first1, last1, first2, last2,
                          ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return merge(::std::checked_base(first1), ::std::checked_base(last1),
                         ::std::checked_base(first2), ::std::checked_base(last2),
                         destination, predication,
                         iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        TBidirectionIterator buffered_rotate(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last,
                                             TDifference count1, TDifference count2, TempIterator<T>& tempBuffer) {
            if (count1 <= count2 && count1 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(first, middle, tempBuffer._Init());
                ::stdext::unchecked_copy(middle, last, first);
                return (::stdext::unchecked_copy_backward(tempBuffer.first(), tempBuffer.last(), last));
            } else if (count2 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(middle, last, tempBuffer._Init());
                ::stdext::unchecked_copy_backward(first, middle, last);
                return (::stdext::unchecked_copy(tempBuffer.first(), tempBuffer.last(), first));
            } else {
                std::rotate(first, middle, last);
                std::advance(first, count2);
                return (first);
            }
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3> inline
        TBidirectionIterator3 merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                                             TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, range_checked_iterator_tag) {
            for (; ;)
                if (first1 == last1)
                    return (::stdext::unchecked_copy_backward(first2, last2, destination));
                else if (first2 == last2)
                    return (::stdext::unchecked_copy_backward(first1, last1, destination));
                else if (debug_lt(*--last2, *--last1, L"..\\ce\\include\\algorithm", 2714))
                    *--destination = *last1, ++last2;
                else
                    *--destination = *last2, ++last1;
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                       TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination) {
            return merge_backward(first1, last1, first2, last2, destination, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                       TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination) {
            return merge_backward(first1, last1, first2, last2, destination, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        void buffered_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last,
                            TDifference count1, TDifference count2, TempIterator<T>& tempBuffer) {
            if (count1 + count2 == 2) {
                if (debug_lt(*middle, *first, L"..\\ce\\include\\algorithm", 2760))
                    std::iter_swap(first, middle);
            } else if (count1 <= count2 && count1 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(first, middle, tempBuffer._Init());
                ::stdext::unchecked_merge(tempBuffer.first(), tempBuffer.last(), middle, last, first);
            } else if (count2 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(middle, last, tempBuffer._Init());
                ::stdext::unchecked_merge_backward(first, middle, tempBuffer.first(), tempBuffer.last(), last);
            } else {
                TBidirectionIterator firstN, lastN;
                TDifference count1n, count2n;

                if (count2 < count1) {
                    count1n = count1 / 2, count2n = 0;
                    firstN = first;
                    std::advance(firstN, count1n);
                    lastN = std::lower_bound(middle, last, *firstN);
                    distance(middle, lastN, count2n);
                } else {
                    count1n = 0, count2n = count2 / 2;
                    lastN = middle;
                    std::advance(lastN, count2n);
                    firstN = std::upper_bound(first, middle, *lastN);
                    distance(first, firstN, count1n);
                }

                TBidirectionIterator middleN = buffered_rotate(firstN, middle, lastN, count1 - count1n, count2n, tempBuffer);
                buffered_merge(first, firstN, middleN, count1n, count2n, tempBuffer);
                buffered_merge(middleN, lastN, last, count1 - count1n, count2 - count2n, tempBuffer);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        void inplace_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last, TDifference*, T*) {
            debug_order(first, middle, L"..\\ce\\include\\algorithm", 2811);
            debug_order(middle, last, L"..\\ce\\include\\algorithm", 2812);
            TDifference count1 = 0;
            distance(first, middle, count1);
            TDifference count2 = 0;
            distance(middle, last, count2);
            TempIterator<T> tempBuffer(count1 < count2 ? count1 : count2);
            buffered_merge(first, middle, last, count1, count2, tempBuffer);
        }

        template<class TBidirectionIterator> inline
        void inplace_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last) {
            if (first != middle && middle != last)
                inplace_merge(::std::checked_base(first), ::std::checked_base(middle), ::std::checked_base(last),
                              DistanceType(first), ValueType(first));
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication> inline
        TBidirectionIterator3 merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                                             TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication, range_checked_iterator_tag) {
            for (; ;)
                if (first1 == last1)
                    return (::stdext::unchecked_copy_backward(first2, last2, destination));
                else if (first2 == last2)
                    return (::stdext::unchecked_copy_backward(first1, last1, destination));
                else if (debug_lt_pred(predication, *--last2, *--last1, L"..\\ce\\include\\algorithm", 2843))
                    *--destination = *last1, ++last2;
                else
                    *--destination = *last2, ++last1;
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1, TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication) {
            return merge_backward(first1, last1, first2, last2, destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1, TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication) {
            return merge_backward(first1, last1, first2, last2, destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TDifference, class T, class TPredication> inline
        void buffered_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last,
                            TDifference count1, TDifference count2,
                            TempIterator<T>& tempBuffer, TPredication predication) {
            if (count1 + count2 == 2) {
                if (debug_lt_pred(predication, *middle, *first, L"..\\ce\\include\\algorithm", 2890))
                    std::iter_swap(first, middle);
            } else if (count1 <= count2 && count1 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(first, middle, tempBuffer._Init());
                ::stdext::unchecked_merge(tempBuffer.first(), tempBuffer.last(),
                                          middle, last, first, predication);
            } else if (count2 <= tempBuffer.maxLength()) {
                ::stdext::unchecked_copy(middle, last, tempBuffer._Init());
                ::stdext::unchecked_merge_backward(first, middle, tempBuffer.first(), tempBuffer.last(),
                                                   last, predication);
            } else {
                TBidirectionIterator firstN, lastN;
                TDifference count1n, count2n;

                if (count2 < count1) {
                    count1n = count1 / 2, count2n = 0;
                    firstN = first;
                    std::advance(firstN, count1n);
                    lastN = lower_bound(middle, last, *firstN, predication);
                    distance(middle, lastN, count2n);
                } else {
                    count1n = 0, count2n = count2 / 2;
                    lastN = middle;
                    std::advance(lastN, count2n);
                    firstN = upper_bound(first, middle, *lastN, predication);
                    distance(first, firstN, count1n);
                }

                TBidirectionIterator middleN = buffered_rotate(firstN, middle, lastN, count1 - count1n, count2n, tempBuffer);
                buffered_merge(first, firstN, middleN, count1n, count2n, tempBuffer, predication);
                buffered_merge(middleN, lastN, last, count1 - count1n, count2 - count2n, tempBuffer, predication);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T, class TPredication> inline
        void inplace_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last, TPredication predication, TDifference*, T*) {
            debug_order(first, middle, predication, L"..\\ce\\include\\algorithm", 2941);
            debug_order(middle, last, predication, L"..\\ce\\include\\algorithm", 2942);
            TDifference count1 = 0;
            distance(first, middle, count1);
            TDifference count2 = 0;
            distance(middle, last, count2);
            TempIterator<T> tempBuffer(count1 < count2 ? count1 : count2);
            buffered_merge(first, middle, last, count1, count2, tempBuffer, predication);
        }

        template<class TBidirectionIterator, class TPredication> inline
        void inplace_merge(TBidirectionIterator first, TBidirectionIterator middle, TBidirectionIterator last, TPredication predication) {
            if (first != middle && middle != last)
                inplace_merge(::std::checked_base(first), ::std::checked_base(middle), ::std::checked_base(last), predication,
                              DistanceType(first), ValueType(first));
        }

        template<class TBidirectionIterator, class T> inline
        void insertion_sort1(TBidirectionIterator first, TBidirectionIterator last, T*) {
            if (first != last)
                for (TBidirectionIterator next = first; ++next != last;) {
                    TBidirectionIterator next1 = next;
                    T value = *next;

                    if (debug_lt(value, *first, L"..\\ce\\include\\algorithm", 2972)) {
                        ::stdext::unchecked_copy_backward(first, next, ++next1);
                        *first = value;
                    } else {
                        for (TBidirectionIterator first1 = next1;
                                debug_lt(value, *--first1, L"..\\ce\\include\\algorithm", 2980);
                                next1 = first1)
                            * next1 = *first1;

                        *next1 = value;
                    }
                }
        }

        template<class TBidirectionIterator> inline
        void insertion_sort(TBidirectionIterator first, TBidirectionIterator last) {
            std::insertion_sort1(first, last, ValueType(first));
        }

        template<class TRandomIterator> inline
        void med3(TRandomIterator first, TRandomIterator middle, TRandomIterator last) {
            if (debug_lt(*middle, *first, L"..\\ce\\include\\algorithm", 2997))
                std::iter_swap(middle, first);

            if (debug_lt(*last, *middle, L"..\\ce\\include\\algorithm", 2999))
                std::iter_swap(last, middle);

            if (debug_lt(*middle, *first, L"..\\ce\\include\\algorithm", 3001))
                std::iter_swap(middle, first);
        }

        template<class TRandomIterator> inline
        void median(TRandomIterator first, TRandomIterator middle, TRandomIterator last) {
            if (40 < last - first) {
                size_t step = (last - first + 1) / 8;
                std::med3(first, first + step, first + 2 * step);
                std::med3(middle - step, middle, middle + step);
                std::med3(last - 2 * step, last - step, last);
                std::med3(first + step, middle, last - step);
            } else
                std::med3(first, middle, last);
        }

        template<class TRandomIterator> inline
        pair<TRandomIterator, TRandomIterator> unguarded_partition(TRandomIterator first, TRandomIterator last) {
            TRandomIterator middle = first + (last - first) / 2;
            std::median(first, middle, last - 1);
            TRandomIterator pFirst = middle;
            TRandomIterator pLast = pFirst + 1;

            while (first < pFirst
                    && !debug_lt(*(pFirst - 1), *pFirst, L"..\\ce\\include\\algorithm", 3029)
                    && !(*pFirst < *(pFirst - 1)))
                --pFirst;

            while (pLast < last
                    && !debug_lt(*pLast, *pFirst, L"..\\ce\\include\\algorithm", 3033)
                    && !(*pFirst < *pLast))
                ++pLast;

            TRandomIterator gFirst = pLast;
            TRandomIterator gLast = pFirst;

            for (; ;) {
                for (; gFirst < last; ++gFirst)
                    if (debug_lt(*pFirst, *gFirst, L"..\\ce\\include\\algorithm", 3043))
                        ;
                    else if (*gFirst < *pFirst)
                        break;
                    else
                        std::iter_swap(pLast++, gFirst);

                for (; first < gLast; --gLast)
                    if (debug_lt(*(gLast - 1), *pFirst, L"..\\ce\\include\\algorithm", 3050))
                        ;
                    else if (*pFirst < *(gLast - 1))
                        break;
                    else
                        std::iter_swap(--pFirst, gLast - 1);

                if (gLast == first && gFirst == last)
                    return (pair<TRandomIterator, TRandomIterator>(pFirst, pLast));

                if (gLast == first) {
                    if (pLast != gFirst)
                        std::iter_swap(pFirst, pLast);

                    ++pLast;
                    std::iter_swap(pFirst++, gFirst++);
                } else if (gFirst == last) {
                    if (--gLast != --pFirst)
                        std::iter_swap(gLast, pFirst);

                    std::iter_swap(pFirst, --pLast);
                } else
                    std::iter_swap(gFirst++, --gLast);
            }
        }

        template<class TRandomIterator, class TDifference> inline
        void sort(TRandomIterator first, TRandomIterator last, TDifference ideal) {
            TDifference count;

            for (; SORT_MAX < (count = last - first) && 0 < ideal;) {
                pair<TRandomIterator, TRandomIterator> middle =
                    std::unguarded_partition(first, last);
                ideal /= 2, ideal += ideal / 2;

                if (middle.first - first < last - middle.second) {
                    std::sort(first, middle.first, ideal);
                    first = middle.second;
                } else {
                    std::sort(middle.second, last, ideal);
                    last = middle.first;
                }
            }

            if (SORT_MAX < count) {
                std::make_heap(first, last);
                std::sort_heap(first, last);
            } else if (1 < count)
                std::insertion_sort(first, last);
        }

        template<class TRandomIterator> inline
        void sort(TRandomIterator first, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3112);
            std::sort(::std::checked_base(first), ::std::checked_base(last), last - first);
        }

        template<class TBidirectionIterator, class TPredication, class T> inline
        void insertion_sort1(TBidirectionIterator first, TBidirectionIterator last, TPredication predication, T*) {
            if (first != last)
                for (TBidirectionIterator next = first; ++next != last;) {
                    TBidirectionIterator next1 = next;
                    T value = *next;

                    if (debug_lt_pred(predication, value, *first, L"..\\ce\\include\\algorithm", 3128)) {
                        ::stdext::unchecked_copy_backward(first, next, ++next1);
                        *first = value;
                    } else {
                        for (TBidirectionIterator first1 = next1;
                                debug_lt_pred(predication, value, *--first1, L"..\\ce\\include\\algorithm", 3136);
                                next1 = first1)
                            * next1 = *first1;

                        *next1 = value;
                    }
                }
        }

        template<class TBidirectionIterator, class TPredication> inline
        void insertion_sort(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            std::insertion_sort1(first, last, predication, ValueType(first));
        }

        template<class TRandomIterator, class TPredication> inline
        void med3(TRandomIterator first, TRandomIterator middle, TRandomIterator last, TPredication predication) {
            if (debug_lt_pred(predication, *middle, *first, L"..\\ce\\include\\algorithm", 3155))
                std::iter_swap(middle, first);

            if (debug_lt_pred(predication, *last, *middle, L"..\\ce\\include\\algorithm", 3157))
                std::iter_swap(last, middle);

            if (debug_lt_pred(predication, *middle, *first, L"..\\ce\\include\\algorithm", 3159))
                std::iter_swap(middle, first);
        }

        template<class TRandomIterator, class TPredication> inline
        void median(TRandomIterator first, TRandomIterator middle, TRandomIterator last, TPredication predication) {
            if (40 < last - first) {
                size_t step = (last - first + 1) / 8;
                std::med3(first, first + step, first + 2 * step, predication);
                std::med3(middle - step, middle, middle + step, predication);
                std::med3(last - 2 * step, last - step, last, predication);
                std::med3(first + step, middle, last - step, predication);
            } else
                std::med3(first, middle, last, predication);
        }

        template<class TRandomIterator, class TPredication> inline
        pair<TRandomIterator, TRandomIterator> unguarded_partition(TRandomIterator first, TRandomIterator last,
                TPredication predication) {
            TRandomIterator middle = first + (last - first) / 2;
            std::median(first, middle, last - 1, predication);
            TRandomIterator pFirst = middle;
            TRandomIterator pLast = pFirst + 1;

            while (first < pFirst
                    && !debug_lt_pred(predication, *(pFirst - 1), *pFirst, L"..\\ce\\include\\algorithm", 3190)
                    && !predication(*pFirst, *(pFirst - 1)))
                --pFirst;

            while (pLast < last
                    && !debug_lt_pred(predication, *pLast, *pFirst, L"..\\ce\\include\\algorithm", 3194)
                    && !predication(*pFirst, *pLast))
                ++pLast;

            TRandomIterator gFirst = pLast;
            TRandomIterator gLast = pFirst;

            for (; ;) {
                for (; gFirst < last; ++gFirst)
                    if (debug_lt_pred(predication, *pFirst, *gFirst, L"..\\ce\\include\\algorithm", 3204))
                        ;
                    else if (predication(*gFirst, *pFirst))
                        break;
                    else
                        std::iter_swap(pLast++, gFirst);

                for (; first < gLast; --gLast)
                    if (debug_lt_pred(predication, *(gLast - 1), *pFirst, L"..\\ce\\include\\algorithm", 3211))
                        ;
                    else if (predication(*pFirst, *(gLast - 1)))
                        break;
                    else
                        std::iter_swap(--pFirst, gLast - 1);

                if (gLast == first && gFirst == last)
                    return (pair<TRandomIterator, TRandomIterator>(pFirst, pLast));

                if (gLast == first) {
                    if (pLast != gFirst)
                        std::iter_swap(pFirst, pLast);

                    ++pLast;
                    std::iter_swap(pFirst++, gFirst++);
                } else if (gFirst == last) {
                    if (--gLast != --pFirst)
                        std::iter_swap(gLast, pFirst);

                    std::iter_swap(pFirst, --pLast);
                } else
                    std::iter_swap(gFirst++, --gLast);
            }
        }

        template<class TRandomIterator, class TDifference, class TPredication> inline
        void sort(TRandomIterator first, TRandomIterator last, TDifference ideal, TPredication predication) {
            TDifference count;

            for (; SORT_MAX < (count = last - first) && 0 < ideal;) {
                pair<TRandomIterator, TRandomIterator> middle =
                    std::unguarded_partition(first, last, predication);
                ideal /= 2, ideal += ideal / 2;

                if (middle.first - first < last - middle.second) {
                    std::sort(first, middle.first, ideal, predication);
                    first = middle.second;
                } else {
                    std::sort(middle.second, last, ideal, predication);
                    last = middle.first;
                }
            }

            if (SORT_MAX < count) {
                std::make_heap(first, last, predication);
                std::sort_heap(first, last, predication);
            } else if (1 < count)
                std::insertion_sort(first, last, predication);
        }

        template<class TRandomIterator, class TPredication> inline
        void sort(TRandomIterator first, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3275);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 3276);
            std::sort(::std::checked_base(first), ::std::checked_base(last), last - first, predication);
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference> inline
        void chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                           TDifference chunk, TDifference count, range_checked_iterator_tag) {
            for (TDifference chunk2 = chunk * 2; chunk2 <= count; count -= chunk2) {
                TBidirectionIterator middle1 = first;
                std::advance(middle1, chunk);
                TBidirectionIterator middle2 = middle1;
                std::advance(middle2, chunk);
                destination = ::stdext::unchecked_merge(first, middle1, middle1, middle2, destination);
                first = middle2;
            }

            if (count <= chunk)
                ::stdext::unchecked_copy(first, last, destination);
            else {
                TBidirectionIterator middle = first;
                std::advance(middle, chunk);
                ::stdext::unchecked_merge(first, middle, middle, last, destination);
            }
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                      TDifference chunk, TDifference count) {
            chunked_merge(first, last, destination, chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutElement, class TDifference, size_t size>
        inline
        void chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size],
                           TDifference chunk, TDifference count) {
            chunked_merge(first, last, ::stdext::make_checked_array_iterator(destination, size), chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                      TDifference chunk, TDifference count) {
            chunked_merge(first, last, destination, chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        void buffered_merge_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference count,
                                 TempIterator<T>& tempBuffer) {
            TBidirectionIterator middle = first;

            for (TDifference Nleft = count; SORT_MAX <= Nleft; Nleft -= SORT_MAX) {
                TBidirectionIterator middleEnd = middle;
                std::advance(middleEnd, (int)SORT_MAX);
                std::insertion_sort(middle, middleEnd);
                middle = middleEnd;
            }

            std::insertion_sort(middle, last);

            for (TDifference chunk = SORT_MAX; chunk < count; chunk *= 2) {
                ::stdext::unchecked_chunked_merge(first, last, tempBuffer._Init(),
                                                  chunk, count);
                ::stdext::unchecked_chunked_merge(tempBuffer.first(), tempBuffer.last(), first,
                                                  chunk *= 2, count);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference count,
                         TempIterator<T>& tempBuffer) {
            if (count <= SORT_MAX)
                std::insertion_sort(first, last);
            else {
                TDifference count2 = (count + 1) / 2;
                TBidirectionIterator middle = first;
                std::advance(middle, count2);

                if (count2 <= tempBuffer.maxLength()) {
                    buffered_merge_sort(first, middle, count2, tempBuffer);
                    buffered_merge_sort(middle, last, count - count2, tempBuffer);
                } else {
                    stable_sort(first, middle, count2, tempBuffer);
                    stable_sort(middle, last, count - count2, tempBuffer);
                }

                buffered_merge(first, middle, last, count2, count - count2, tempBuffer);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference*, T*) {
            TDifference count = 0;
            distance(first, last, count);
            TempIterator<T> tempBuffer((count + 1) / 2);
            stable_sort(first, last, count, tempBuffer);
        }

        template<class TBidirectionIterator> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3418);

            if (first != last) {
                stable_sort(::std::checked_base(first), ::std::checked_base(last), DistanceType(first), ValueType(first));
            }
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication> inline
        void chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                           TDifference chunk, TDifference count, TPredication predication, range_checked_iterator_tag) {
            for (TDifference chunk2 = chunk * 2; chunk2 <= count; count -= chunk2) {
                TBidirectionIterator middle1 = first;
                std::advance(middle1, chunk);
                TBidirectionIterator middle2 = middle1;
                std::advance(middle2, chunk);
                destination = ::stdext::unchecked_merge(first, middle1, middle1, middle2, destination, predication);
                first = middle2;
            }

            if (count <= chunk)
                ::stdext::unchecked_copy(first, last, destination);
            else {
                TBidirectionIterator middle1 = first;
                std::advance(middle1, chunk);
                ::stdext::unchecked_merge(first, middle1, middle1, last, destination, predication);
            }
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                      TDifference chunk, TDifference count, TPredication predication) {
            chunked_merge(first, last, destination, chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutElement, class TDifference, class TPredication, size_t size>
        inline
        void chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size],
                           TDifference chunk, TDifference count, TPredication predication) {
            chunked_merge(first, last, ::stdext::make_checked_array_iterator(destination, size), chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination,
                      TDifference chunk, TDifference count, TPredication predication) {
            chunked_merge(first, last, destination, chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TDifference, class T, class TPredication> inline
        void buffered_merge_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference count,
                                 TempIterator<T>& tempBuffer, TPredication predication) {
            TBidirectionIterator middle = first;

            for (TDifference Nleft = count; SORT_MAX <= Nleft; Nleft -= SORT_MAX) {
                TBidirectionIterator middleN = middle;
                std::advance(middleN, (int)SORT_MAX);
                std::insertion_sort(middle, middleN, predication);
                middle = middleN;
            }

            std::insertion_sort(middle, last, predication);

            for (TDifference chunk = SORT_MAX; chunk < count; chunk *= 2) {
                ::stdext::unchecked_chunked_merge(first, last, tempBuffer._Init(), chunk, count, predication);
                ::stdext::unchecked_chunked_merge(tempBuffer.first(), tempBuffer.last(), first, chunk *= 2, count, predication);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T, class TPredication> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference count, TempIterator<T>& tempBuffer, TPredication predication) {
            if (count <= SORT_MAX)
                std::insertion_sort(first, last, predication);
            else {
                TDifference count2 = (count + 1) / 2;
                TBidirectionIterator middle = first;
                std::advance(middle, count2);

                if (count2 <= tempBuffer.maxLength()) {
                    buffered_merge_sort(first, middle, count2, tempBuffer, predication);
                    buffered_merge_sort(middle, last, count - count2, tempBuffer, predication);
                } else {
                    stable_sort(first, middle, count2, tempBuffer, predication);
                    stable_sort(middle, last, count - count2, tempBuffer, predication);
                }

                buffered_merge(first, middle, last, count2, count - count2, tempBuffer, predication);
            }
        }

        template<class TBidirectionIterator, class TDifference, class T, class TPredication> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last, TDifference*, T*, TPredication predication) {
            TDifference count = 0;
            distance(first, last, count);
            TempIterator<T> tempBuffer((count + 1) / 2);
            stable_sort(first, last, count, tempBuffer, predication);
        }

        template<class TBidirectionIterator, class TPredication> inline
        void stable_sort(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3569);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 3570);

            if (first != last) {
                stable_sort(::std::checked_base(first), ::std::checked_base(last), DistanceType(first), ValueType(first), predication);
            }
        }

        template<class TRandomIterator, class T> inline
        void partial_sort(TRandomIterator first, TRandomIterator middle, TRandomIterator last, T*) {
            debug_range(first, middle, L"..\\ce\\include\\algorithm", 3583);
            debug_range(middle, last, L"..\\ce\\include\\algorithm", 3584);
            std::make_heap(first, middle);

            for (TRandomIterator next = middle; next < last; ++next)
                if (debug_lt(*next, *first, L"..\\ce\\include\\algorithm", 3588))
                    pop_heap(first, middle, next, T(*next),
                             DistanceType(first));

            std::sort_heap(first, middle);
        }

        template<class TRandomIterator> inline
        void partial_sort(TRandomIterator first, TRandomIterator middle, TRandomIterator last) {
            partial_sort(::std::checked_base(first), ::std::checked_base(middle), ::std::checked_base(last), ValueType(first));
        }

        template<class TRandomIterator, class T, class TPredication> inline
        void partial_sort(TRandomIterator first, TRandomIterator middle, TRandomIterator last, TPredication predication, T*) {
            debug_range(first, middle, L"..\\ce\\include\\algorithm", 3607);
            debug_range(middle, last, L"..\\ce\\include\\algorithm", 3608);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 3609);
            std::make_heap(first, middle, predication);

            for (TRandomIterator next = middle; next < last; ++next)
                if (debug_lt_pred(predication, *next, *first, L"..\\ce\\include\\algorithm", 3613))
                    pop_heap(first, middle, next, T(*next), predication,
                             DistanceType(first));

            std::sort_heap(first, middle, predication);
        }

        template<class TRandomIterator, class TPredication> inline
        void partial_sort(TRandomIterator first, TRandomIterator middle, TRandomIterator last, TPredication predication) {
            partial_sort(::std::checked_base(first), ::std::checked_base(middle), ::std::checked_base(last), predication, ValueType(first));
        }

        template<class TInputInterator, class TRandomIterator, class TDifference, class T> inline
        TRandomIterator partial_sort_copy(TInputInterator first1, TInputInterator last1,
                                          TRandomIterator first2, TRandomIterator last2, TDifference*, T*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 3634);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 3635);
            TRandomIterator middle2 = first2;

            for (; first1 != last1 && middle2 != last2; ++first1, ++middle2)
                *middle2 = *first1;

            std::make_heap(first2, middle2);

            for (; first1 != last1; ++first1)
                if (debug_lt(*first1, *first2, L"..\\ce\\include\\algorithm", 3642))
                    std::adjust_heap(first2, TDifference(0), TDifference(middle2 - first2),
                                     T(*first1));

            std::sort_heap(first2, middle2);
            return (middle2);
        }

        template<class TInputInterator, class TRandomIterator> inline
        TRandomIterator partial_sort_copy(TInputInterator first1, TInputInterator last1,
                                          TRandomIterator first2, TRandomIterator last2) {
            if (first1 != last1 && first2 != last2)
                ::std::checked_assign_from_base(first2, partial_sort_copy(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), DistanceType(first2), ValueType(first1)));

            return (first2);
        }

        template<class TInputInterator, class TRandomIterator, class TDifference, class T, class TPredication> inline
        TRandomIterator partial_sort_copy(TInputInterator first1, TInputInterator last1,
                                          TRandomIterator first2, TRandomIterator last2, TPredication predication, TDifference*, T*) {
            debug_range(first1, last1, L"..\\ce\\include\\algorithm", 3671);
            debug_range(first2, last2, L"..\\ce\\include\\algorithm", 3672);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 3673);
            TRandomIterator middle2 = first2;

            for (; first1 != last1 && middle2 != last2; ++first1, ++middle2)
                *middle2 = *first1;

            std::make_heap(first2, middle2, predication);

            for (; first1 != last1; ++first1)
                if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\algorithm", 3680))
                    std::adjust_heap(first2, TDifference(0), TDifference(middle2 - first2),
                                     T(*first1), predication);

            std::sort_heap(first2, middle2, predication);
            return (middle2);
        }

        template<class TInputInterator, class TRandomIterator, class TPredication> inline
        TRandomIterator partial_sort_copy(TInputInterator first1, TInputInterator last1,
                                          TRandomIterator first2, TRandomIterator last2, TPredication predication) {
            if (first1 != last1 && first2 != last2)
                ::std::checked_assign_from_base(first2, partial_sort_copy(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), predication, DistanceType(first2), ValueType(first1)));

            return (first2);
        }

        template<class TRandomIterator> inline
        void Nth_element(TRandomIterator first, TRandomIterator Nth, TRandomIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3705);

            for (; SORT_MAX < last - first;) {
                pair<TRandomIterator, TRandomIterator> middle =
                    std::unguarded_partition(first, last);

                if (middle.second <= Nth)
                    first = middle.second;
                else if (middle.first <= Nth)
                    return;
                else
                    last = middle.first;
            }

            std::insertion_sort(first, last);
        }

        template<class TRandomIterator> inline
        void nth_element(TRandomIterator first, TRandomIterator Nth, TRandomIterator last) {
            Nth_element(::std::checked_base(first), ::std::checked_base(Nth), ::std::checked_base(last));
        }

        template<class TRandomIterator, class TPredication> inline
        void Nth_element(TRandomIterator first, TRandomIterator Nth, TRandomIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 3733);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 3734);

            for (; SORT_MAX < last - first;) {
                pair<TRandomIterator, TRandomIterator> middle =
                    std::unguarded_partition(first, last, predication);

                if (middle.second <= Nth)
                    first = middle.second;
                else if (middle.first <= Nth)
                    return;
                else
                    last = middle.first;
            }

            std::insertion_sort(first, last, predication);
        }

        template<class TRandomIterator, class TPredication> inline
        void nth_element(TRandomIterator first, TRandomIterator Nth, TRandomIterator last, TPredication predication) {
            Nth_element(::std::checked_base(first), ::std::checked_base(Nth), ::std::checked_base(last), predication);
        }

        template<class TInputIterator1, class TInputIterator2> inline
        bool includes(TInputIterator1 first1, TInputIterator1 last1,
                      TInputIterator2 first2, TInputIterator2 last2) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 3764);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 3765);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt(*first2, *first1, L"..\\ce\\include\\algorithm", 3767))
                    return (false);
                else if (*first1 < *first2)
                    ++first1;
                else
                    ++first1, ++first2;

            return (first2 == last2);
        }

        template<class TInputIterator1, class TInputIterator2> inline
        bool includes(TInputIterator1 first1, TInputIterator1 last1,
                      TInputIterator2 first2, TInputIterator2 last2) {
            return includes(::std::checked_base(first1), ::std::checked_base(last1),
                            ::std::checked_base(first2), ::std::checked_base(last2));
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication> inline
        bool includes(TInputIterator1 first1, TInputIterator1 last1,
                      TInputIterator2 first2, TInputIterator2 last2, TPredication predication) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 3792);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 3793);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt_pred(predication, *first2, *first1, L"..\\ce\\include\\algorithm", 3795))
                    return (false);
                else if (predication(*first1, *first2))
                    ++first1;
                else
                    ++first1, ++first2;

            return (first2 == last2);
        }

        template<class TInputIterator1, class TInputIterator2, class TPredication> inline
        bool includes(TInputIterator1 first1, TInputIterator1 last1,
                      TInputIterator2 first2, TInputIterator2 last2, TPredication predication) {
            return includes(::std::checked_base(first1), ::std::checked_base(last1),
                            ::std::checked_base(first2), ::std::checked_base(last2), predication);
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator set_union(TInputIterator1 first1, TInputIterator1 last1,
                                   TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, range_checked_iterator_tag) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 3821);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 3822);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 3823);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt(*first1, *first2, L"..\\ce\\include\\algorithm", 3825))
                    *destination++ = *first1, ++first1;
                else if (*first2 < *first1)
                    *destination++ = *first2, ++first2;
                else
                    *destination++ = *first1, ++first1, ++first2;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_union(TInputIterator1 first1, TInputIterator1 last1,
                  TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_union(::std::checked_base(first1), ::std::checked_base(last1),
                             ::std::checked_base(first2), ::std::checked_base(last2),
                             destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* set_union(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (set_union(first1, last1, first2, last2,
                              ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_union(TInputIterator1 first1, TInputIterator1 last1,
                  TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_union(::std::checked_base(first1), ::std::checked_base(last1),
                             ::std::checked_base(first2), ::std::checked_base(last2),
                             destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator set_union(TInputIterator1 first1, TInputIterator1 last1,
                                   TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication, range_checked_iterator_tag) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 3892);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 3893);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 3894);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\algorithm", 3896))
                    *destination++ = *first1, ++first1;
                else if (predication(*first2, *first1))
                    *destination++ = *first2, ++first2;
                else
                    *destination++ = *first1, ++first1, ++first2;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_union(::std::checked_base(first1), ::std::checked_base(last1),
                             ::std::checked_base(first2), ::std::checked_base(last2),
                             destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* set_union(TInputIterator1 first1, TInputIterator1 last1,
                               TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (set_union(first1, last1, first2, last2,
                              ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_union(::std::checked_base(first1), ::std::checked_base(last1),
                             ::std::checked_base(first2), ::std::checked_base(last2),
                             destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                          TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, range_checked_iterator_tag) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 3965);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 3966);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 3967);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt(*first1, *first2, L"..\\ce\\include\\algorithm", 3969))
                    ++first1;
                else if (*first2 < *first1)
                    ++first2;
                else
                    *destination++ = *first1++, ++first2;

            return (destination);
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                      TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (set_intersection(first1, last1, first2, last2,
                                     ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                          TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication, range_checked_iterator_tag) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 4035);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 4036);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 4037);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\algorithm", 4039))
                    ++first1;
                else if (predication(*first2, *first1))
                    ++first2;
                else
                    *destination++ = *first1++, ++first2;

            return (destination);
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                      TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (set_intersection(first1, last1, first2, last2,
                                     ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator set_difference(TInputIterator1 first1, TInputIterator1 last1,
                                        TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, range_checked_iterator_tag) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 4107);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 4108);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 4109);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt(*first1, *first2, L"..\\ce\\include\\algorithm", 4111))
                    *destination++ = *first1, ++first1;
                else if (*first2 < *first1)
                    ++first2;
                else
                    ++first1, ++first2;

            return (::stdext::unchecked_copy(first1, last1, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_difference(TInputIterator1 first1, TInputIterator1 last1,
                       TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                  ::std::checked_base(first2), ::std::checked_base(last2),
                                  destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* set_difference(TInputIterator1 first1, TInputIterator1 last1,
                                    TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (set_difference(first1, last1, first2, last2,
                                   ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                  ::std::checked_base(first2), ::std::checked_base(last2),
                                  destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator set_difference(TInputIterator1 first1, TInputIterator1 last1,
                                        TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication, range_checked_iterator_tag) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 4177);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 4178);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 4179);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\algorithm", 4181))
                    *destination++ = *first1, ++first1;
                else if (predication(*first2, *first1))
                    ++first2;
                else
                    ++first1, ++first2;

            return (::stdext::unchecked_copy(first1, last1, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                  ::std::checked_base(first2), ::std::checked_base(last2),
                                  destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* set_difference(TInputIterator1 first1, TInputIterator1 last1,
                                    TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (set_difference(first1, last1, first2, last2,
                                   ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                  ::std::checked_base(first2), ::std::checked_base(last2),
                                  destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, range_checked_iterator_tag) {
            debug_order(first1, last1, L"..\\ce\\include\\algorithm", 4249);
            debug_order(first2, last2, L"..\\ce\\include\\algorithm", 4250);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 4251);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt(*first1, *first2, L"..\\ce\\include\\algorithm", 4253))
                    *destination++ = *first1, ++first1;
                else if (*first2 < *first1)
                    *destination++ = *first2, ++first2;
                else
                    ++first1, ++first2;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                            ::std::checked_base(first2), ::std::checked_base(last2),
                                            destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                                              TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (set_symmetric_difference(first1, last1, first2, last2,
                                             ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                            ::std::checked_base(first2), ::std::checked_base(last2),
                                            destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication, range_checked_iterator_tag) {
            debug_order(first1, last1, predication, L"..\\ce\\include\\algorithm", 4320);
            debug_order(first2, last2, predication, L"..\\ce\\include\\algorithm", 4321);
            debug_pointer(destination, L"..\\ce\\include\\algorithm", 4322);

            for (; first1 != last1 && first2 != last2;)
                if (debug_lt_pred(predication, *first1, *first2, L"..\\ce\\include\\algorithm", 4324))
                    *destination++ = *first1, ++first1;
                else if (predication(*first2, *first1))
                    *destination++ = *first2, ++first2;
                else
                    ++first1, ++first2;

            destination = ::stdext::unchecked_copy(first1, last1, destination);
            return (::stdext::unchecked_copy(first2, last2, destination));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                            ::std::checked_base(first2), ::std::checked_base(last2),
                                            destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                                              TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (set_symmetric_difference(first1, last1, first2, last2,
                                             ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                            ::std::checked_base(first2), ::std::checked_base(last2),
                                            destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator> inline
        TForwardIterator max_element(TForwardIterator first, TForwardIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4390);
            TForwardIterator found = first;

            if (first != last)
                for (; ++first != last;)
                    if (debug_lt(*found, *first, L"..\\ce\\include\\algorithm", 4394))
                        found = first;

            return (found);
        }

        template<class TForwardIterator> inline
        TForwardIterator max_element(TForwardIterator first, TForwardIterator last) {
            ::std::checked_assign_from_base(first, max_element(::std::checked_base(first), ::std::checked_base(last)));
            return (first);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator max_element(TForwardIterator first, TForwardIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4412);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 4413);
            TForwardIterator found = first;

            if (first != last)
                for (; ++first != last;)
                    if (debug_lt_pred(predication, *found, *first, L"..\\ce\\include\\algorithm", 4417))
                        found = first;

            return (found);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator max_element(TForwardIterator first, TForwardIterator last, TPredication predication) {
            ::std::checked_assign_from_base(first, max_element(::std::checked_base(first), ::std::checked_base(last), predication));
            return (first);
        }

        template<class TForwardIterator> inline
        TForwardIterator min_element(TForwardIterator first, TForwardIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4435);
            TForwardIterator found = first;

            if (first != last)
                for (; ++first != last;)
                    if (debug_lt(*first, *found, L"..\\ce\\include\\algorithm", 4439))
                        found = first;

            return (found);
        }

        template<class TForwardIterator> inline
        TForwardIterator min_element(TForwardIterator first, TForwardIterator last) {
            ::std::checked_assign_from_base(first, min_element(::std::checked_base(first), ::std::checked_base(last)));
            return (first);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator min_element(TForwardIterator first, TForwardIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4457);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 4458);
            TForwardIterator found = first;

            if (first != last)
                for (; ++first != last;)
                    if (debug_lt_pred(predication, *first, *found, L"..\\ce\\include\\algorithm", 4462))
                        found = first;

            return (found);
        }

        template<class TForwardIterator, class TPredication> inline
        TForwardIterator min_element(TForwardIterator first, TForwardIterator last, TPredication predication) {
            ::std::checked_assign_from_base(first, min_element(::std::checked_base(first), ::std::checked_base(last), predication));
            return (first);
        }

        template<class TBidirectionIterator> inline
        bool next_permutation(TBidirectionIterator first, TBidirectionIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4480);
            TBidirectionIterator next = last;

            if (first == last || first == --next)
                return (false);

            for (; ;) {
                TBidirectionIterator next1 = next;

                if (debug_lt(*--next, *next1, L"..\\ce\\include\\algorithm", 4488)) {
                    TBidirectionIterator middle = last;

                    for (; !debug_lt(*next, *--middle, L"..\\ce\\include\\algorithm", 4491);)
                        ;

                    std::iter_swap(next, middle);
                    std::reverse(next1, last);
                    return (true);
                }

                if (next == first) {
                    std::reverse(first, last);
                    return (false);
                }
            }
        }

        template<class TBidirectionIterator> inline
        bool next_permutation(TBidirectionIterator first, TBidirectionIterator last) {
            return next_permutation(::std::checked_base(first), ::std::checked_base(last));
        }

        template<class TBidirectionIterator, class TPredication> inline
        bool next_permutation(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4517);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 4518);
            TBidirectionIterator next = last;

            if (first == last || first == --next)
                return (false);

            for (; ;) {
                TBidirectionIterator next1 = next;

                if (debug_lt_pred(predication, *--next, *next1, L"..\\ce\\include\\algorithm", 4526)) {
                    TBidirectionIterator middle = last;

                    for (; !debug_lt_pred(predication, *next, *--middle, L"..\\ce\\include\\algorithm", 4529);)
                        ;

                    std::iter_swap(next, middle);
                    std::reverse(next1, last);
                    return (true);
                }

                if (next == first) {
                    std::reverse(first, last);
                    return (false);
                }
            }
        }

        template<class TBidirectionIterator, class TPredication> inline
        bool next_permutation(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            return next_permutation(::std::checked_base(first), ::std::checked_base(last), predication);
        }

        template<class TBidirectionIterator> inline
        bool prev_permutation(TBidirectionIterator first, TBidirectionIterator last) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4555);
            TBidirectionIterator next = last;

            if (first == last || first == --next)
                return (false);

            for (; ;) {
                TBidirectionIterator next1 = next;

                if (debug_lt(*next1, *--next, L"..\\ce\\include\\algorithm", 4562)) {
                    TBidirectionIterator middle = last;

                    for (; !debug_lt(*--middle, *next, L"..\\ce\\include\\algorithm", 4565);)
                        ;

                    std::iter_swap(next, middle);
                    std::reverse(next1, last);
                    return (true);
                }

                if (next == first) {
                    std::reverse(first, last);
                    return (false);
                }
            }
        }

        template<class TBidirectionIterator> inline
        bool prev_permutation(TBidirectionIterator first, TBidirectionIterator last) {
            return prev_permutation(::std::checked_base(first), ::std::checked_base(last));
        }

        template<class TBidirectionIterator, class TPredication> inline
        bool prev_permutation(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            debug_range(first, last, L"..\\ce\\include\\algorithm", 4591);
            debug_pointer(predication, L"..\\ce\\include\\algorithm", 4592);
            TBidirectionIterator next = last;

            if (first == last || first == --next)
                return (false);

            for (; ;) {
                TBidirectionIterator next1 = next;

                if (debug_lt_pred(predication, *next1, *--next, L"..\\ce\\include\\algorithm", 4600)) {
                    TBidirectionIterator middle = last;

                    for (; !debug_lt_pred(predication, *--middle, *next, L"..\\ce\\include\\algorithm", 4603);)
                        ;

                    std::iter_swap(next, middle);
                    std::reverse(next1, last);
                    return (true);
                }

                if (next == first) {
                    std::reverse(first, last);
                    return (false);
                }
            }
        }

        template<class TBidirectionIterator, class TPredication> inline
        bool prev_permutation(TBidirectionIterator first, TBidirectionIterator last, TPredication predication) {
            return prev_permutation(::std::checked_base(first), ::std::checked_base(last), predication);
        }
    }

    namespace stdext {
        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        TOutputInterator unchecked_transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function) {
            return ::std::transform(::std::checked_base(first), ::std::checked_base(last), destination, function,
                                    ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function) {
            return ::std::transform(::std::checked_base(first), ::std::checked_base(last), destination, function,
                                    ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TFunction1, size_t size>
        inline
        TOutElement* checked_transform(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TFunction1 function) {
            return (checked_transform(first, last,
                                      ::stdext::make_checked_array_iterator(destination, size), function).base());
        }
        template<class TInputInterator, class TOutputInterator, class TFunction1>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputInterator first, TInputInterator last, TOutputInterator destination, TFunction1 function) {
            return ::std::transform(::std::checked_base(first), ::std::checked_base(last), destination, function,
                                    ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        TOutputInterator unchecked_transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2,
                                             TOutputInterator destination, TFunction2 function) {
            return ::std::transform(::std::checked_base(first1), ::std::checked_base(last1), first2, destination, function,
                                    ::std::iterator_random(first1, first2), ::std::iterator_random(first1, destination),
                                    ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag());
        }
        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        typename ::std::enable_if < ::std::is_checked_iterator< TInputIterator2 >::result && ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TOutputInterator destination, TFunction2 function) {
            return ::std::transform(::std::checked_base(first1), ::std::checked_base(last1), first2, destination, function,
                                    ::std::iterator_random(first1, first2), ::std::iterator_random(first1, destination),
                                    ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInElement2, class TOutElement, class TFunction2, size_t SizeFirst2, size_t DestinationSize>
        inline
        TOutElement* checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2],
                                       TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (checked_transform(first1, last1,
                                      ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                                      ::stdext::make_checked_array_iterator(destination, DestinationSize),
                                      function).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TFunction2, size_t DestinationSize>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TInputIterator2 >::result, TOutElement* >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (::std::transform(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                     ::stdext::make_checked_array_iterator(destination, DestinationSize), function,
                                     ::std::iterator_random(first1, first2), ::std::iterator_category(first1),
                                     ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TFunction2, size_t DestinationSize>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result, TOutElement* >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TOutElement(&destination)[DestinationSize], TFunction2 function) {
            return (::std::transform(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                     ::stdext::make_checked_array_iterator(destination, DestinationSize), function,
                                     ::std::iterator_random(first1, first2), ::std::iterator_category(first1),
                                     ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()).base());
        }

        template<class TInputIterator1, class TInElement2, class TOutputInterator, class TFunction2, size_t SizeFirst2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2], TOutputInterator destination, TFunction2 function) {
            return (::std::transform(::std::checked_base(first1), ::std::checked_base(last1),
                                     ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                                     destination, function,
                                     ::std::iterator_category(first1), ::std::iterator_random(first1, destination),
                                     ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()));
        }

        template<class TInputIterator1, class TInElement2, class TOutputInterator, class TFunction2, size_t SizeFirst2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInElement2(&first2)[SizeFirst2], TOutputInterator destination, TFunction2 function) {
            return (::std::transform(::std::checked_base(first1), ::std::checked_base(last1),
                                     ::stdext::make_checked_array_iterator(first2, SizeFirst2),
                                     destination, function,
                                     ::std::iterator_category(first1), ::std::iterator_random(first1, destination),
                                     ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag()));
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TFunction2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TInputIterator2 >::result || !::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_transform(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TOutputInterator destination, TFunction2 function) {
            return ::std::transform(::std::checked_base(first1), ::std::checked_base(last1), first2, destination, function,
                                    ::std::iterator_random(first1, first2), ::std::iterator_random(first1, destination),
                                    ::std::range_checked_iterator_tag(), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        TForwardIterator2 unchecked_swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2) {
            return ::std::swap_ranges(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                      ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TForwardIterator2 >::result, TForwardIterator2 >::result
        checked_swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2) {
            return ::std::swap_ranges(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                      ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator1, class TForwardElement2, size_t size>
        inline
        TForwardElement2* checked_swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardElement2(&first2)[size]) {
            return (checked_swap_ranges(first1, last1, ::stdext::make_checked_array_iterator(first2, size)).base());
        }

        template<class TForwardIterator1, class TForwardIterator2>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TForwardIterator2 >::result, TForwardIterator2 >::result
        checked_swap_ranges(TForwardIterator1 first1, TForwardIterator1 last1, TForwardIterator2 first2) {
            return ::std::swap_ranges(::std::checked_base(first1), ::std::checked_base(last1), first2,
                                      ::std::iterator_random(first1, first2), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        TOutputInterator unchecked_replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                                                const T& oldValue, const T& newValue) {
            return ::std::replace_copy(::std::checked_base(first), ::std::checked_base(last), destination, oldValue, newValue,
                                       ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination,
                             const T& oldValue, const T& newValue) {
            return ::std::replace_copy(::std::checked_base(first), ::std::checked_base(last), destination, oldValue, newValue,
                                       ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class T, size_t size>
        inline
        TOutElement* checked_replace_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], const T& oldValue, const T& newValue) {
            return (checked_replace_copy(first, last,
                                         ::stdext::make_checked_array_iterator(destination, size),
                                         oldValue, newValue).base());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_replace_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& oldValue, const T& newValue) {
            return ::std::replace_copy(::std::checked_base(first), ::std::checked_base(last), destination, oldValue, newValue,
                                       ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T> inline
        TOutputInterator unchecked_replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, const T& value) {
            return ::std::replace_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, value,
                                          ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, const T& value) {
            return ::std::replace_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, value,
                                          ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TPredication, class T, size_t size>
        inline
        TOutElement* checked_replace_copy_if(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication, const T& value) {
            return (checked_replace_copy_if(first, last,
                                            ::stdext::make_checked_array_iterator(destination, size),
                                            predication, value).base());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_replace_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication, const T& value) {
            return ::std::replace_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, value,
                                          ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TOutputInterator, class TDifference, class TFunction0> inline
        void unchecked_generate_n(TOutputInterator destination, TDifference count, TFunction0 function) {
            ::std::generate_n(destination, count, function,
                              ::std::iterator_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TOutputInterator, class TDifference, class TFunction0> inline
        void checked_generate_n(TOutputInterator destination, TDifference count, TFunction0 function, typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TDifference >::result = 0) {
            ::std::generate_n(destination, count, function,
                              ::std::iterator_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TOutElement, class TDifference, class TFunction0, size_t size>
        inline
        void checked_generate_n(TOutElement(&destination)[size], TDifference count, TFunction0 function) {
            checked_generate_n(::stdext::make_checked_array_iterator(destination, size), count, function);
        }

        template<class TOutputInterator, class TDifference, class TFunction0> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        void checked_generate_n(TOutputInterator destination, TDifference count, TFunction0 function, typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TDifference >::result = 0) {
            ::std::generate_n(destination, count, function,
                              ::std::iterator_category(destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        TOutputInterator unchecked_remove_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& value) {
            return ::std::remove_copy(::std::checked_base(first), ::std::checked_base(last), destination, value,
                                      ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_remove_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& value) {
            return ::std::remove_copy(::std::checked_base(first), ::std::checked_base(last), destination, value, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class T, size_t size>
        inline
        TOutElement* checked_remove_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], const T& value) {
            return (checked_remove_copy(first, last,
                                        ::stdext::make_checked_array_iterator(destination, size),
                                        value).base());
        }

        template<class TInputInterator, class TOutputInterator, class T> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_remove_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, const T& value) {
            return ::std::remove_copy(::std::checked_base(first), ::std::checked_base(last), destination, value, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return ::std::remove_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication,
                                         ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return ::std::remove_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_remove_copy_if(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication) {
            return (checked_remove_copy_if(first, last,
                                           ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_remove_copy_if(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return ::std::remove_copy_if(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputInterator, class TOutputInterator> inline
        TOutputInterator unchecked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (first == last ? destination :
                    ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                       ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }
        template<class TInputInterator, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (first == last ? destination :
                    ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutElement, size_t size>
        inline
        TOutElement* checked_unique_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size]) {
            return (first == last ? destination :
                    (checked_unique_copy(first, last, ::stdext::make_checked_array_iterator(destination, size)).base()));
        }

        template<class TInputInterator, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination) {
            return (first == last ? destination :
                    ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return (first == last ? destination
                    : ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, predication,
                                         ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return (first == last ? destination
                    : ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TInputInterator, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_unique_copy(TInputInterator first, TInputInterator last, TOutElement(&destination)[size], TPredication predication) {
            return (first == last ? destination
                    : (checked_unique_copy(first, last, ::stdext::make_checked_array_iterator(destination, size), predication).base()));
        }

        template<class TInputInterator, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_unique_copy(TInputInterator first, TInputInterator last, TOutputInterator destination, TPredication predication) {
            return (first == last ? destination
                    : ::std::unique_copy(::std::checked_base(first), ::std::checked_base(last), destination, predication, ::std::iterator_category(first), ::std::range_checked_iterator_tag()));
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        TOutputInterator unchecked_reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination) {
            return ::std::reverse_copy(::std::checked_base(first), ::std::checked_base(last), destination,
                                       ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination) {
            return ::std::reverse_copy(::std::checked_base(first), ::std::checked_base(last), destination, ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutElement, size_t size>
        inline
        TOutElement* checked_reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size]) {
            return (checked_reverse_copy(first, last, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TBidirectionIterator, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_reverse_copy(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination) {
            return ::std::reverse_copy(::std::checked_base(first), ::std::checked_base(last), destination, ::std::iterator_random(first, destination), ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TOutputInterator> inline
        TOutputInterator unchecked_rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination) {
            return ::std::rotate_copy(first, middle, last, destination, ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TOutputInterator>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination) {
            return ::std::rotate_copy(first, middle, last, destination, ::std::range_checked_iterator_tag());
        }

        template<class TForwardIterator, class TOutElement, size_t size>
        inline
        TOutElement* checked_rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutElement(&destination)[size]) {
            return (checked_rotate_copy(first, middle, last, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TForwardIterator, class TOutputInterator>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_rotate_copy(TForwardIterator first, TForwardIterator middle, TForwardIterator last, TOutputInterator destination) {
            return ::std::rotate_copy(first, middle, last, destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator unchecked_merge(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination)  {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1),
                                ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* checked_merge(TInputIterator1 first1, TInputIterator1 last1,
                                   TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (checked_merge(first1, last1, first2, last2,
                                  ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1),
                                ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_merge(TInputIterator1 first1, TInputIterator1 last1,
                                         TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination, predication,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_merge(TInputIterator1 first1, TInputIterator1 last1,
                      TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1),
                                ::std::checked_base(first2), ::std::checked_base(last2),
                                destination, predication,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_merge(TInputIterator1 first1, TInputIterator1 last1,
                                   TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (checked_merge(first1, last1, first2, last2,
                                  ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_merge(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::merge(::std::checked_base(first1), ::std::checked_base(last1),
                                ::std::checked_base(first2), ::std::checked_base(last2),
                                destination, predication,
                                ::std::iterator_random(first1, first2, destination), ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3>
        inline
        TBidirectionIterator3 unchecked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result checked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        checked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1, TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication>
        inline
        TBidirectionIterator3 unchecked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,
                TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        checked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1,  TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator1, class TBidirectionIterator2, class TBidirectionIterator3, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TBidirectionIterator3 >::result, TBidirectionIterator3 >::result
        checked_merge_backward(TBidirectionIterator1 first1, TBidirectionIterator1 last1, TBidirectionIterator2 first2, TBidirectionIterator2 last2, TBidirectionIterator3 destination, TPredication predication) {
            return ::std::merge_backward(first1, last1, first2, last2, destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference>
        inline
        void unchecked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count) {
            ::std::chunked_merge(first, last, destination, chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count) {
            ::std::chunked_merge(first, last, destination, chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutElement, class TDifference, size_t size>
        inline
        void checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size], TDifference chunk, TDifference count) {
            ::std::chunked_merge(first, last, ::stdext::make_checked_array_iterator(destination, size), chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count) {
            ::std::chunked_merge(first, last, destination, chunk, count, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication>
        inline
        void unchecked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count, TPredication predication) {
            ::std::chunked_merge(first, last, destination, chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication>
        inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count, TPredication predication) {
            ::std::chunked_merge(first, last, destination, chunk, count, predication, ::std::range_checked_iterator_tag());
        }
        template<class TBidirectionIterator, class TOutElement, class TDifference, class TPredication, size_t size>
        inline
        void checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutElement(&destination)[size],
                                   TDifference chunk, TDifference count, TPredication predication) {
            ::std::chunked_merge(first, last, ::stdext::make_checked_array_iterator(destination, size), chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TBidirectionIterator, class TOutputInterator, class TDifference, class TPredication>
        inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, void >::result
        checked_chunked_merge(TBidirectionIterator first, TBidirectionIterator last, TOutputInterator destination, TDifference chunk, TDifference count, TPredication predication) {
            ::std::chunked_merge(first, last, destination, chunk, count, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator unchecked_set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                    ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* checked_set_union(TInputIterator1 first1, TInputIterator1 last1,
                                       TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (checked_set_union(first1, last1, first2, last2,
                                      ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_set_union(TInputIterator1 first1, TInputIterator1 last1,
                                             TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination, predication,
                                    ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_set_union(TInputIterator1 first1, TInputIterator1 last1,
                                       TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (checked_set_union(first1, last1, first2, last2,
                                      ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_union(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_union(::std::checked_base(first1), ::std::checked_base(last1),
                                    ::std::checked_base(first2), ::std::checked_base(last2),
                                    destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator unchecked_set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                           ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                           ::std::checked_base(first2), ::std::checked_base(last2),
                                           destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                              TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (checked_set_intersection(first1, last1, first2, last2,
                                             ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                           ::std::checked_base(first2), ::std::checked_base(last2),
                                           destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination, predication,
                                           ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                           ::std::checked_base(first2), ::std::checked_base(last2),
                                           destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1,
                                              TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (checked_set_intersection(first1, last1, first2, last2,
                                             ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_intersection(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_intersection(::std::checked_base(first1), ::std::checked_base(last1),
                                           ::std::checked_base(first2), ::std::checked_base(last2),
                                           destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator unchecked_set_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                         ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                         ::std::checked_base(first2), ::std::checked_base(last2),
                                         destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* checked_set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (checked_set_difference(first1, last1, first2, last2, ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                         ::std::checked_base(first2), ::std::checked_base(last2),
                                         destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_set_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination, predication,
                                         ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                         ::std::checked_base(first2), ::std::checked_base(last2),
                                         destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_set_difference(TInputIterator1 first1, TInputIterator1 last1,
                                            TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (checked_set_difference(first1, last1, first2, last2,
                                           ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                         ::std::checked_base(first2), ::std::checked_base(last2),
                                         destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        TOutputInterator unchecked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination,
                                                   ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                                   ::std::checked_base(first2), ::std::checked_base(last2),
                                                   destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, size_t size>
        inline
        TOutElement* checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size]) {
            return (checked_set_symmetric_difference(first1, last1, first2, last2,
                    ::stdext::make_checked_array_iterator(destination, size)).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                                   ::std::checked_base(first2), ::std::checked_base(last2),
                                                   destination, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        TOutputInterator unchecked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1), ::std::checked_base(first2), ::std::checked_base(last2), destination, predication,
                                                   ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        typename ::std::enable_if< ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                                   ::std::checked_base(first2), ::std::checked_base(last2),
                                                   destination, predication, ::std::range_checked_iterator_tag());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutElement, class TPredication, size_t size>
        inline
        TOutElement* checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1,
                TInputIterator2 first2, TInputIterator2 last2, TOutElement(&destination)[size], TPredication predication) {
            return (checked_set_symmetric_difference(first1, last1, first2, last2,
                    ::stdext::make_checked_array_iterator(destination, size), predication).base());
        }

        template<class TInputIterator1, class TInputIterator2, class TOutputInterator, class TPredication> inline
        __declspec(deprecated("Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. " "To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"))
        typename ::std::enable_if < ! ::std::is_checked_iterator< TOutputInterator >::result, TOutputInterator >::result
        checked_set_symmetric_difference(TInputIterator1 first1, TInputIterator1 last1, TInputIterator2 first2, TInputIterator2 last2, TOutputInterator destination, TPredication predication) {
            return ::std::set_symmetric_difference(::std::checked_base(first1), ::std::checked_base(last1),
                                                   ::std::checked_base(first2), ::std::checked_base(last2),
                                                   destination, predication, ::std::range_checked_iterator_tag());
        }
    }

