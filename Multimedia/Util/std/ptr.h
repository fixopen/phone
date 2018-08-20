namespace boost {
    typedef long long long_long_type;
    typedef unsigned long long ulong_long_type;

    template<class T> inline void checked_delete(T* x) {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete x;
    }

    template<class T> inline void checked_array_delete(T* x) {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete [] x;
    }

    template<class T> struct checked_deleter {
        typedef void result_type;
        typedef T* argument_type;
        void operator()(T* x) const {
            boost::checked_delete(x);
        }
    };

    template<class T> struct checked_array_deleter {
        typedef void result_type;
        typedef T* argument_type;
        void operator()(T* x) const {
            boost::checked_array_delete(x);
        }
    };

    template<class T> class scoped_ptr {
    private:
        T* px;
        scoped_ptr(scoped_ptr const&);
        scoped_ptr& operator=(scoped_ptr const&);
        typedef scoped_ptr<T> this_type;
        void operator==(scoped_ptr const&) const;
        void operator!=(scoped_ptr const&) const;
    public:
        typedef T element_type;
        explicit scoped_ptr(T* p = 0): px(p)
        {    }
        explicit scoped_ptr(std::auto_ptr<T> p): px(p.release())
        {    }
        ~scoped_ptr() {
            boost::checked_delete(px);
        }

        void reset(T* p = 0) {
            (void)((p == 0 || p != px) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_ptr.hpp" L"(" L"85" L"):\r\n" L"p == 0 || p != px" L"\r\n"), __debugbreak(), 0));
            this_type(p).swap(*this);
        }

        T& operator*() const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_ptr.hpp" L"(" L"91" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return *px;
        }

        T* operator->() const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_ptr.hpp" L"(" L"97" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return px;
        }

        T* get() const {
            return px;
        }

        typedef T* this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const {
            return px == 0 ? 0 : &this_type::px;
        }

        bool operator!() const {
            return px == 0;
        }

        void swap(scoped_ptr& b) {
            T* tmp = b.px;
            b.px = px;
            px = tmp;
        }
    };

    template<class T> inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) {
        a.swap(b);
    }

    template<class T> inline T* get_pointer(scoped_ptr<T> const& p) {
        return p.get();
    }

    template<class T> class scoped_array {
    private:
        T* px;
        scoped_array(scoped_array const&);
        scoped_array& operator=(scoped_array const&);
        typedef scoped_array<T> this_type;
        void operator==(scoped_array const&) const;
        void operator!=(scoped_array const&) const;
    public:
        typedef T element_type;
        explicit scoped_array(T* p = 0) : px(p)
        {    }
        ~scoped_array() {
            boost::checked_array_delete(px);
        }

        void reset(T* p = 0) {
            (void)((p == 0 || p != px) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_array.hpp" L"(" L"73" L"):\r\n" L"p == 0 || p != px" L"\r\n"), __debugbreak(), 0));
            this_type(p).swap(*this);
        }

        T& operator[](std::ptrdiff_t i) const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_array.hpp" L"(" L"79" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            (void)((i >= 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/scoped_array.hpp" L"(" L"80" L"):\r\n" L"i >= 0" L"\r\n"), __debugbreak(), 0));
            return px[i];
        }

        T* get() const {
            return px;
        }

        typedef T* this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const {
            return px == 0 ? 0 : &this_type::px;
        }

        bool operator!() const {
            return px == 0;
        }

        void swap(scoped_array& b)  {
            T* tmp = b.px;
            b.px = px;
            px = tmp;
        }
    };

    template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) {
        a.swap(b);
    }

    namespace exception_detail {
        template <class T>
        class refcount_ptr {
        public:
            refcount_ptr(): px_(0) {
            }
            ~refcount_ptr() {
                release();
            }
            refcount_ptr(refcount_ptr const& x): px_(x.px_) {
                add_ref();
            }
            refcount_ptr& operator=(refcount_ptr const& x) {
                adopt(x.px_);
                return *this;
            }
            void adopt(T* px) {
                release();
                px_ = px;
                add_ref();
            }
            T* get() const {
                return px_;
            }
        private:
            T* px_;
            void add_ref() {
                if (px_)
                    px_->add_ref();
            }
            void release() {
                if (px_)
                    px_->release();
            }
        };
    }
    template <class Tag, class T>
    class error_info;
    typedef error_info<struct tag_throw_function, char const*> throw_function;
    typedef error_info<struct tag_throw_file, char const*> throw_file;
    typedef error_info<struct tag_throw_line, int> throw_line;
    template <>
    class error_info<tag_throw_function, char const*> {
    public:
        typedef char const* value_type;
        value_type v_;
        explicit error_info(value_type v): v_(v) {
        }
    };
    template <>
    class
            error_info<tag_throw_file, char const*> {
    public:
        typedef char const* value_type;
        value_type v_;
        explicit error_info(value_type v): v_(v) {
        }
    };
    template <>
    class
            error_info<tag_throw_line, int> {
    public:
        typedef int value_type;
        value_type v_;
        explicit error_info(value_type v): v_(v) {
        }
    };
    template <class E, class Tag, class T>
    E const& operator<<(E const&, error_info<Tag, T> const&);
    class exception;
    template <class>
    class shared_ptr;
    namespace exception_detail {
        class error_info_base;
        struct type_info_;
        struct error_info_container {
            virtual char const* diagnostic_information() const = 0;
            virtual shared_ptr<error_info_base const> get(type_info_ const&) const = 0;
            virtual void set(shared_ptr<error_info_base const> const&, type_info_ const&) = 0;
            virtual void add_ref() const = 0;
            virtual void release() const = 0;
        protected:
            virtual
            ~error_info_container() throw() {
            }
        };
        template <class>
        struct get_info;
        template <>
        struct get_info<throw_function>;
        template <>
        struct get_info<throw_file>;
        template <>
        struct get_info<throw_line>;
        char const* get_diagnostic_information(exception const&);
    }
    class exception {
    protected:
        exception():
            throw_function_(0),
            throw_file_(0),
            throw_line_(-1) {
        }
        virtual ~exception() throw() = 0;
    private:
        template <class E>
        friend E const& operator<<(E const& x, throw_function const& y) {
            x.throw_function_ = y.v_;
            return x;
        }
        template <class E>
        friend E const& operator<<(E const& x, throw_file const& y) {
            x.throw_file_ = y.v_;
            return x;
        }
        template <class E>
        friend E const& operator<<(E const& x, throw_line const& y) {
            x.throw_line_ = y.v_;
            return x;
        }
        friend char const* exception_detail::get_diagnostic_information(exception const&);
        template <class E, class Tag, class T>
        friend E const& operator<<(E const&, error_info<Tag, T> const&);
        template <class>
        friend struct exception_detail::get_info;
        friend struct exception_detail::get_info<throw_function>;
        friend struct exception_detail::get_info<throw_file>;
        friend struct exception_detail::get_info<throw_line>;
        mutable exception_detail::refcount_ptr<exception_detail::error_info_container> data_;
        mutable char const* throw_function_;
        mutable char const* throw_file_;
        mutable int throw_line_;
    };
    inline exception::~exception() throw() {
    }
    namespace exception_detail {
        template <class T>
        struct error_info_injector: public T, public exception {
            explicit error_info_injector(T const& x): T(x) {
            }
            ~error_info_injector() throw() {
            }
        };
        struct large_size {
            char c[256];
        };
        large_size dispatch(exception*);
        struct small_size { };
        small_size dispatch(void*);
        template <class, int>
        struct enable_error_info_helper;
        template <class T>
        struct enable_error_info_helper<T, sizeof(large_size)> {
            typedef T type;
        };
        template <class T>
        struct enable_error_info_helper<T, sizeof(small_size)> {
            typedef error_info_injector<T> type;
        };
        template <class T>
        struct enable_error_info_return_type {
            typedef typename enable_error_info_helper<T, sizeof(dispatch((T*)0))>::type type;
        };
    }
    template <class T>
    inline
    typename
    exception_detail::enable_error_info_return_type<T>::type
    enable_error_info(T const& x) {
        typedef typename exception_detail::enable_error_info_return_type<T>::type rt;
        return rt(x);
    }
    namespace exception_detail {
        class clone_base {
        public:
            virtual clone_base const* clone() const = 0;
            virtual void rethrow() const = 0;
            virtual ~clone_base() throw() {
            }
        };
        inline void copy_boost_exception(exception* a, exception const* b) {
            *a = *b;
        }
        inline void copy_boost_exception(void*, void const*) {
        }
        template <class T>
        class clone_impl: public T, public clone_base {
        public:
            explicit clone_impl(T const& x): T(x) {
                copy_boost_exception(this, &x);
            }
            ~clone_impl() throw() {
            }
        private:
            clone_base const* clone() const {
                return new clone_impl(*this);
            }
            void rethrow() const {
                throw* this;
            }
        };
    }
    template <class T>
    inline exception_detail::clone_impl<T>
    enable_current_exception(T const& x) {
        return exception_detail::clone_impl<T>(x);
    }

    namespace detail {
        inline void current_function_helper() {
        }
    }

    inline void throw_exception_assert_compatibility(std::exception const&) { }
    template<class E> inline void throw_exception(E const& e) {
        throw_exception_assert_compatibility(e);
        throw enable_current_exception(enable_error_info(e));
    }

    class bad_weak_ptr: public std::exception {
    public:
        virtual char const* what() const throw() {
            return "tr1::bad_weak_ptr";
        }
    };

    namespace detail {
        typedef std::type_info sp_typeinfo;
    }

    namespace detail {
        class sp_counted_base {
        private:
            sp_counted_base(sp_counted_base const&);
            sp_counted_base& operator= (sp_counted_base const&);
            long use_count_;
            long weak_count_;
        public:
            sp_counted_base(): use_count_(1), weak_count_(1) {
            }
            virtual ~sp_counted_base() {
            }
            virtual void dispose() = 0;
            virtual void destroy() {
                delete this;
            }
            virtual void* get_deleter(sp_typeinfo const& ti) = 0;
            void add_ref_copy() {
                InterlockedIncrement(&use_count_);
            }
            bool add_ref_lock() {
                for (;;) {
                    long tmp = static_cast< long const volatile& >(use_count_);

                    if (tmp == 0)
                        return false;

                    if (InterlockedCompareExchange(&use_count_, tmp + 1, tmp) == tmp)
                        return true;
                }
            }
            void release() {
                if (InterlockedDecrement(&use_count_) == 0) {
                    dispose();
                    weak_release();
                }
            }
            void weak_add_ref() {
                InterlockedIncrement(&weak_count_);
            }
            void weak_release() {
                if (InterlockedDecrement(&weak_count_) == 0) {
                    destroy();
                }
            }
            long use_count() const {
                return static_cast<long const volatile&>(use_count_);
            }
        };
    }

    namespace detail {
        template<class X> class sp_counted_impl_p: public sp_counted_base {
        private:
            X* px_;
            sp_counted_impl_p(sp_counted_impl_p const&);
            sp_counted_impl_p& operator= (sp_counted_impl_p const&);
            typedef sp_counted_impl_p<X> this_type;
        public:
            explicit sp_counted_impl_p(X* px): px_(px)
            {    }
            virtual void dispose() {
                boost::checked_delete(px_);
            }
            virtual void* get_deleter(detail::sp_typeinfo const&) {
                return 0;
            }
        };
        template<class P, class D> class sp_counted_impl_pd: public sp_counted_base {
        private:
            P ptr;
            D del;
            sp_counted_impl_pd(sp_counted_impl_pd const&);
            sp_counted_impl_pd& operator= (sp_counted_impl_pd const&);
            typedef sp_counted_impl_pd<P, D> this_type;
        public:
            sp_counted_impl_pd(P p, D d): ptr(p), del(d) {
            }
            virtual void dispose() {
                del(ptr);
            }
            virtual void* get_deleter(detail::sp_typeinfo const& ti) {
                return ti == typeid(D) ? &reinterpret_cast<char&>(del) : 0;
            }
        };
        template<class P, class D, class A> class sp_counted_impl_pda: public sp_counted_base {
        private:
            P p_;
            D d_;
            A a_;
            sp_counted_impl_pda(sp_counted_impl_pda const&);
            sp_counted_impl_pda& operator= (sp_counted_impl_pda const&);
            typedef sp_counted_impl_pda<P, D, A> this_type;
        public:
            sp_counted_impl_pda(P p, D d, A a): p_(p), d_(d), a_(a) {
            }
            virtual void dispose() {
                d_(p_);
            }
            virtual void destroy() {
                typedef typename A::template rebind< this_type >::other A2;
                A2 a2(a_);
                this->~this_type();
                a2.deallocate(this, 1);
            }
            virtual void* get_deleter(detail::sp_typeinfo const& ti) {
                return ti == typeid(D) ? &reinterpret_cast<char&>(d_) : 0;
            }
        };
    }

    namespace detail {
        struct sp_nothrow_tag {};
        class weak_count;
        class shared_count {
        private:
            sp_counted_base* pi_;
            friend class weak_count;
        public:
            shared_count(): pi_(0) {
            }
            template<class Y> explicit shared_count(Y* p): pi_(0) {
                try {
                    pi_ = new sp_counted_impl_p<Y>(p);
                } catch (...) {
                    boost::checked_delete(p);
                    throw;
                }
            }
            template<class P, class D> shared_count(P p, D d): pi_(0) {
                try {
                    pi_ = new sp_counted_impl_pd<P, D>(p, d);
                } catch (...) {
                    d(p);
                    throw;
                }
            }
            template<class P, class D, class A> shared_count(P p, D d, A a): pi_(0) {
                typedef sp_counted_impl_pda<P, D, A> impl_type;
                typedef typename A::template rebind< impl_type >::other A2;
                A2 a2(a);

                try {
                    pi_ = a2.allocate(1, static_cast< impl_type* >(0));
                    new(static_cast< void* >(pi_)) impl_type(p, d, a);
                } catch (...) {
                    d(p);

                    if (pi_ != 0) {
                        a2.deallocate(static_cast< impl_type* >(pi_), 1);
                    }

                    throw;
                }
            }
            template<class Y>
            explicit shared_count(std::auto_ptr<Y> & r): pi_(new sp_counted_impl_p<Y>(r.get())) {
                r.release();
            }
            ~shared_count() {
                if (pi_ != 0)
                    pi_->release();
            }
            shared_count(shared_count const& r): pi_(r.pi_) {
                if (pi_ != 0)
                    pi_->add_ref_copy();
            }
            explicit shared_count(weak_count const& r);
            shared_count(weak_count const& r, sp_nothrow_tag);
            shared_count& operator= (shared_count const& r) {
                sp_counted_base* tmp = r.pi_;

                if (tmp != pi_) {
                    if (tmp != 0)
                        tmp->add_ref_copy();

                    if (pi_ != 0)
                        pi_->release();

                    pi_ = tmp;
                }

                return *this;
            }
            void swap(shared_count& r) {
                sp_counted_base* tmp = r.pi_;
                r.pi_ = pi_;
                pi_ = tmp;
            }
            long use_count() const {
                return pi_ != 0 ? pi_->use_count() : 0;
            }
            bool unique() const {
                return use_count() == 1;
            }
            bool empty() const {
                return pi_ == 0;
            }
            friend inline bool operator==(shared_count const& a, shared_count const& b) {
                return a.pi_ == b.pi_;
            }
            friend inline bool operator<(shared_count const& a, shared_count const& b) {
                return std::less<sp_counted_base*>()(a.pi_, b.pi_);
            }
            void* get_deleter(sp_typeinfo const& ti) const {
                return pi_ ? pi_->get_deleter(ti) : 0;
            }
        };
        class weak_count {
        private:
            sp_counted_base* pi_;
            friend class shared_count;
        public:
            weak_count(): pi_(0) {
            }
            weak_count(shared_count const& r): pi_(r.pi_) {
                if (pi_ != 0) pi_->weak_add_ref();
            }
            weak_count(weak_count const& r): pi_(r.pi_) {
                if (pi_ != 0) pi_->weak_add_ref();
            }
            ~weak_count() {
                if (pi_ != 0)
                    pi_->weak_release();
            }
            weak_count& operator= (shared_count const& r) {
                sp_counted_base* tmp = r.pi_;

                if (tmp != pi_) {
                    if (tmp != 0)
                        tmp->weak_add_ref();

                    if (pi_ != 0)
                        pi_->weak_release();

                    pi_ = tmp;
                }

                return *this;
            }
            weak_count& operator= (weak_count const& r) {
                sp_counted_base* tmp = r.pi_;

                if (tmp != pi_) {
                    if (tmp != 0)
                        tmp->weak_add_ref();

                    if (pi_ != 0)
                        pi_->weak_release();

                    pi_ = tmp;
                }

                return *this;
            }
            void swap(weak_count& r) {
                sp_counted_base* tmp = r.pi_;
                r.pi_ = pi_;
                pi_ = tmp;
            }
            long use_count() const {
                return pi_ != 0 ? pi_->use_count() : 0;
            }
            bool empty() const {
                return pi_ == 0;
            }
            friend inline bool operator==(weak_count const& a, weak_count const& b) {
                return a.pi_ == b.pi_;
            }
            friend inline bool operator<(weak_count const& a, weak_count const& b) {
                return std::less<sp_counted_base*>()(a.pi_, b.pi_);
            }
        };
        inline shared_count::shared_count(weak_count const& r): pi_(r.pi_) {
            if (pi_ == 0 || !pi_->add_ref_lock()) {
                boost::throw_exception(boost::bad_weak_ptr());
            }
        }
        inline shared_count::shared_count(weak_count const& r, sp_nothrow_tag): pi_(r.pi_) {
            if (pi_ != 0 && !pi_->add_ref_lock()) {
                pi_ = 0;
            }
        }
    }

    namespace detail {
        template< class Y, class T > struct sp_convertible {
            typedef char(&yes)[1];
            typedef char(&no)[2];
            static yes f(T*);
            static no  f(...);
            enum _vt {
                value = sizeof(f((Y*)0)) == sizeof(yes)
            };
        };
        struct sp_empty {
        };
        template< bool > struct sp_enable_if_convertible_impl;
        template<> struct sp_enable_if_convertible_impl<true> {
            typedef sp_empty type;
        };
        template<> struct sp_enable_if_convertible_impl<false> {
        };
        template< class Y, class T > struct sp_enable_if_convertible: public sp_enable_if_convertible_impl< sp_convertible< Y, T >::value > {
        };
    }

    namespace detail {
        extern "C" void __cdecl Sleep(unsigned ms);
        inline void yield(unsigned k) {
            if (k < 4) {
            } else if (k < 32) {
                Sleep(0);
            } else {
                Sleep(1);
            }
        }
    }

    namespace detail {
        class spinlock {
        public:
            long v_;
        public:
            bool try_lock() {
                long r = InterlockedExchange(&v_, 1);
                _ReadWriteBarrier();
                return r == 0;
            }
            void lock() {
                for (unsigned k = 0; !try_lock(); ++k) {
                    boost::detail::yield(k);
                }
            }
            void unlock() {
                _ReadWriteBarrier();
                *const_cast< long volatile* >(&v_) = 0;
            }
        public:
            class scoped_lock {
            private:
                spinlock& sp_;
                scoped_lock(scoped_lock const&);
                scoped_lock& operator=(scoped_lock const&);
            public:
                explicit scoped_lock(spinlock& sp): sp_(sp) {
                    sp.lock();
                }
                ~scoped_lock() {
                    sp_.unlock();
                }
            };
        };
    }

    namespace detail {
        template< int I > class spinlock_pool {
        private:
            static spinlock pool_[ 41 ];
        public:
            static spinlock& spinlock_for(void const* pv) {
                std::size_t i = reinterpret_cast< std::size_t >(pv) % 41;
                return pool_[ i ];
            }
            class scoped_lock {
            private:
                spinlock& sp_;
                scoped_lock(scoped_lock const&);
                scoped_lock& operator=(scoped_lock const&);
            public:
                explicit scoped_lock(void const* pv): sp_(spinlock_for(pv)) {
                    sp_.lock();
                }
                ~scoped_lock() {
                    sp_.unlock();
                }
            };
        };
        template< int I > spinlock spinlock_pool< I >::pool_[ 41 ] = {
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {0}
        };
    }

    enum memory_order {
        memory_order_relaxed = 0,
        memory_order_acquire = 1,
        memory_order_release = 2,
        memory_order_acq_rel = 3,
        memory_order_seq_cst = 7
    };

    template<class T> class shared_ptr;
    template<class T> class weak_ptr;
    template<class T> class enable_shared_from_this;
    namespace detail {
        struct static_cast_tag {};
        struct const_cast_tag {};
        struct dynamic_cast_tag {};
        struct polymorphic_cast_tag {};
        template<class T> struct shared_ptr_traits {
            typedef T& reference;
        };
        template<> struct shared_ptr_traits<void> {
            typedef void reference;
        };
        template<> struct shared_ptr_traits<void const> {
            typedef void reference;
        };
        template<> struct shared_ptr_traits<void volatile> {
            typedef void reference;
        };
        template<> struct shared_ptr_traits<void const volatile> {
            typedef void reference;
        };
        template< class X, class Y, class T > inline void sp_enable_shared_from_this(boost::shared_ptr<X> const* ppx, Y const* py, boost::enable_shared_from_this< T > const* pe) {
            if (pe != 0) {
                pe->_internal_accept_owner(ppx, const_cast< Y* >(py));
            }
        }
        inline void sp_enable_shared_from_this(...) {
        }
        template< class T, class R > struct sp_enable_if_auto_ptr {
        };
        template< class T, class R > struct sp_enable_if_auto_ptr< std::auto_ptr< T >, R > {
            typedef R type;
        };
    }
    template<class T> class shared_ptr {
    private:
        typedef shared_ptr<T> this_type;
    public:
        typedef T element_type;
        typedef T value_type;
        typedef T* pointer;
        typedef typename boost::detail::shared_ptr_traits<T>::reference reference;
        shared_ptr(): px(0), pn() {
        }
        template<class Y>
        explicit shared_ptr(Y* p): px(p), pn(p) {
            boost::detail::sp_enable_shared_from_this(this, p, p);
        }
        template<class Y, class D> shared_ptr(Y* p, D d): px(p), pn(p, d) {
            boost::detail::sp_enable_shared_from_this(this, p, p);
        }
        template<class Y, class D, class A> shared_ptr(Y* p, D d, A a): px(p), pn(p, d, a) {
            boost::detail::sp_enable_shared_from_this(this, p, p);
        }
        template<class Y>
        explicit shared_ptr(weak_ptr<Y> const& r): pn(r.pn) {
            px = r.px;
        }
        template<class Y>
        shared_ptr(weak_ptr<Y> const& r, boost::detail::sp_nothrow_tag): px(0), pn(r.pn, boost::detail::sp_nothrow_tag()) {
            if (!pn.empty()) {
                px = r.px;
            }
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const& r, typename detail::sp_enable_if_convertible<Y, T>::type = detail::sp_empty())
            : px(r.px), pn(r.pn) {
        }
        template< class Y >
        shared_ptr(shared_ptr<Y> const& r, T* p): px(p), pn(r.pn) {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const& r, boost::detail::static_cast_tag): px(static_cast<element_type*>(r.px)), pn(r.pn) {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const& r, boost::detail::const_cast_tag): px(const_cast<element_type*>(r.px)), pn(r.pn) {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const& r, boost::detail::dynamic_cast_tag): px(dynamic_cast<element_type*>(r.px)), pn(r.pn) {
            if (px == 0) {
                pn = boost::detail::shared_count();
            }
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const& r, boost::detail::polymorphic_cast_tag): px(dynamic_cast<element_type*>(r.px)), pn(r.pn) {
            if (px == 0) {
                boost::throw_exception(std::bad_cast());
            }
        }
        template<class Y>
        explicit shared_ptr(std::auto_ptr<Y> & r): px(r.get()), pn() {
            Y* tmp = r.get();
            pn = boost::detail::shared_count(r);
            boost::detail::sp_enable_shared_from_this(this, tmp, tmp);
        }
        template<class Ap>
        explicit shared_ptr(Ap r, typename boost::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0): px(r.get()), pn() {
            typename Ap::element_type* tmp = r.get();
            pn = boost::detail::shared_count(r);
            boost::detail::sp_enable_shared_from_this(this, tmp, tmp);
        }
        shared_ptr& operator=(shared_ptr const& r) {
            this_type(r).swap(*this);
            return *this;
        }
        template<class Y>
        shared_ptr& operator=(shared_ptr<Y> const& r) {
            this_type(r).swap(*this);
            return *this;
        }
        template<class Y>
        shared_ptr& operator=(std::auto_ptr<Y> & r) {
            this_type(r).swap(*this);
            return *this;
        }
        template<class Ap>
        typename boost::detail::sp_enable_if_auto_ptr< Ap, shared_ptr& >::type operator=(Ap r) {
            this_type(r).swap(*this);
            return *this;
        }
        void reset() {
            this_type().swap(*this);
        }
        template<class Y> void reset(Y* p) {
            (void)((p == 0 || p != px) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"./boost/smart_ptr/shared_ptr.hpp" L"(" L"382" L"):\r\n" L"p == 0 || p != px" L"\r\n"), __debugbreak(), 0));
            this_type(p).swap(*this);
        }
        template<class Y, class D> void reset(Y* p, D d) {
            this_type(p, d).swap(*this);
        }
        template<class Y, class D, class A> void reset(Y* p, D d, A a) {
            this_type(p, d, a).swap(*this);
        }
        template<class Y> void reset(shared_ptr<Y> const& r, T* p) {
            this_type(r, p).swap(*this);
        }
        reference operator*() const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_ptr.hpp" L"(" L"403" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return *px;
        }
        T* operator-> () const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_ptr.hpp" L"(" L"409" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return px;
        }
        T* get() const {
            return px;
        }
        typedef T* this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const {
            return px == 0 ? 0 : &this_type::px;
        }
        bool operator!() const {
            return px == 0;
        }
        bool unique() const {
            return pn.unique();
        }
        long use_count() const {
            return pn.use_count();
        }
        void swap(shared_ptr<T> & other) {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
        template<class Y> bool _internal_less(shared_ptr<Y> const& rhs) const {
            return pn < rhs.pn;
        }
        void* _internal_get_deleter(detail::sp_typeinfo const& ti) const {
            return pn.get_deleter(ti);
        }
        bool _internal_equiv(shared_ptr const& r) const {
            return px == r.px && pn == r.pn;
        }
    private:
        template<class Y> friend class shared_ptr;
        template<class Y> friend class weak_ptr;
        T* px;
        boost::detail::shared_count pn;
    };
    template<class T, class U> inline bool operator==(shared_ptr<T> const& a, shared_ptr<U> const& b) {
        return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(shared_ptr<T> const& a, shared_ptr<U> const& b) {
        return a.get() != b.get();
    }
    template<class T, class U> inline bool operator<(shared_ptr<T> const& a, shared_ptr<U> const& b) {
        return a._internal_less(b);
    }
    template<class T> inline void swap(shared_ptr<T> & a, shared_ptr<T> & b) {
        a.swap(b);
    }
    template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::static_cast_tag());
    }
    template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::const_cast_tag());
    }
    template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_static_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::static_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_polymorphic_cast(shared_ptr<U> const& r) {
        return shared_ptr<T>(r, boost::detail::polymorphic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_polymorphic_downcast(shared_ptr<U> const& r) {
        (void)((dynamic_cast<T*>(r.get()) == r.get()) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_ptr.hpp" L"(" L"535" L"):\r\n" L"dynamic_cast<T *>(r.get()) == r.get()" L"\r\n"), __debugbreak(), 0));
        return shared_static_cast<T>(r);
    }
    template<class T> inline T* get_pointer(shared_ptr<T> const& p) {
        return p.get();
    }
    template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const& p) {
        os << p.get();
        return os;
    }
    template<class D, class T> D* get_deleter(shared_ptr<T> const& p) {
        return static_cast<D*>(p._internal_get_deleter(typeid(D)));
    }
    template<class T> inline bool atomic_is_lock_free(shared_ptr<T> const*) {
        return false;
    }
    template<class T> shared_ptr<T> atomic_load(shared_ptr<T> const* p) {
        boost::detail::spinlock_pool<2>::scoped_lock lock(p);
        return *p;
    }
    template<class T> inline shared_ptr<T> atomic_load_explicit(shared_ptr<T> const* p, memory_order) {
        return atomic_load(p);
    }
    template<class T> void atomic_store(shared_ptr<T> * p, shared_ptr<T> r) {
        boost::detail::spinlock_pool<2>::scoped_lock lock(p);
        p->swap(r);
    }
    template<class T> inline void atomic_store_explicit(shared_ptr<T> * p, shared_ptr<T> r, memory_order) {
        atomic_store(p, r);
    }
    template<class T> shared_ptr<T> atomic_exchange(shared_ptr<T> * p, shared_ptr<T> r) {
        boost::detail::spinlock& sp = boost::detail::spinlock_pool<2>::spinlock_for(p);
        sp.lock();
        p->swap(r);
        sp.unlock();
        return r;
    }
    template<class T> shared_ptr<T> atomic_exchange_explicit(shared_ptr<T> * p, shared_ptr<T> r, memory_order) {
        return atomic_exchange(p, r);
    }
    template<class T> bool atomic_compare_exchange(shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w) {
        boost::detail::spinlock& sp = boost::detail::spinlock_pool<2>::spinlock_for(p);
        sp.lock();

        if (p->_internal_equiv(*v)) {
            p->swap(w);
            sp.unlock();
            return true;
        } else {
            shared_ptr<T> tmp(*p);
            sp.unlock();
            tmp.swap(*v);
            return false;
        }
    }
    template<class T> inline bool atomic_compare_exchange_explicit(shared_ptr<T> * p, shared_ptr<T> * v, shared_ptr<T> w, memory_order , memory_order) {
        return atomic_compare_exchange(p, v, w);
    }

#pragma warning(pop)

    template<class T> class shared_array {
    private:
        typedef checked_array_deleter<T> deleter;
        typedef shared_array<T> this_type;
    public:
        typedef T element_type;
        explicit shared_array(T* p = 0): px(p), pn(p, deleter()) {
        }
        template<class D> shared_array(T* p, D d): px(p), pn(p, d) {
        }
        void reset(T* p = 0) {
            (void)((p == 0 || p != px) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_array.hpp" L"(" L"76" L"):\r\n" L"p == 0 || p != px" L"\r\n"), __debugbreak(), 0));
            this_type(p).swap(*this);
        }
        template <class D> void reset(T* p, D d) {
            this_type(p, d).swap(*this);
        }
        T& operator[](std::ptrdiff_t i) const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_array.hpp" L"(" L"87" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            (void)((i >= 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/shared_array.hpp" L"(" L"88" L"):\r\n" L"i >= 0" L"\r\n"), __debugbreak(), 0));
            return px[i];
        }

        T* get() const {
            return px;
        }
        typedef T* this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const {
            return px == 0 ? 0 : &this_type::px;
        }
        bool operator!() const {
            return px == 0;
        }
        bool unique() const {
            return pn.unique();
        }
        long use_count() const {
            return pn.use_count();
        }
        void swap(shared_array<T> & other) {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
    private:
        T* px;
        detail::shared_count pn;
    };
    template<class T> inline bool operator==(shared_array<T> const& a, shared_array<T> const& b) {
        return a.get() == b.get();
    }
    template<class T> inline bool operator!=(shared_array<T> const& a, shared_array<T> const& b) {
        return a.get() != b.get();
    }
    template<class T> inline bool operator<(shared_array<T> const& a, shared_array<T> const& b) {
        return std::less<T*>()(a.get(), b.get());
    }
    template<class T> void swap(shared_array<T> & a, shared_array<T> & b) {
        a.swap(b);
    }

#pragma warning(push)
#pragma warning(disable:4284)

    template<class T> class weak_ptr {
    private:
        typedef weak_ptr<T> this_type;
    public:
        typedef T element_type;
        weak_ptr(): px(0), pn() {
        }
        template<class Y>
        weak_ptr(weak_ptr<Y> const& r, typename detail::sp_enable_if_convertible<Y, T>::type = detail::sp_empty())
            : pn(r.pn) {
            px = r.lock().get();
        }
        template<class Y>
        weak_ptr(shared_ptr<Y> const& r, typename detail::sp_enable_if_convertible<Y, T>::type = detail::sp_empty())
            : px(r.px), pn(r.pn) {
        }
        template<class Y>
        weak_ptr& operator=(weak_ptr<Y> const& r) {
            px = r.lock().get();
            pn = r.pn;
            return *this;
        }
        template<class Y>
        weak_ptr& operator=(shared_ptr<Y> const& r) {
            px = r.px;
            pn = r.pn;
            return *this;
        }
        shared_ptr<T> lock() const {
            return shared_ptr<element_type>(*this, boost::detail::sp_nothrow_tag());
        }
        long use_count() const {
            return pn.use_count();
        }
        bool expired() const {
            return pn.use_count() == 0;
        }
        void reset() {
            this_type().swap(*this);
        }
        void swap(this_type& other) {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
        void _internal_assign(T* px2, boost::detail::shared_count const& pn2) {
            px = px2;
            pn = pn2;
        }
        template<class Y> bool _internal_less(weak_ptr<Y> const& rhs) const {
            return pn < rhs.pn;
        }
    private:
        template<class Y> friend class weak_ptr;
        template<class Y> friend class shared_ptr;
        T* px;
        boost::detail::weak_count pn;
    };
    template<class T, class U> inline bool operator<(weak_ptr<T> const& a, weak_ptr<U> const& b) {
        return a._internal_less(b);
    }
    template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b) {
        a.swap(b);
    }

#pragma warning(pop)
#pragma warning(push)
#pragma warning(disable:4284)

    template<class T> class intrusive_ptr {
    private:
        typedef intrusive_ptr this_type;
    public:
        typedef T element_type;
        intrusive_ptr(): px(0) {
        }
        intrusive_ptr(T* p, bool add_ref = true): px(p) {
            if (px != 0 && add_ref)
                intrusive_ptr_add_ref(px);
        }
        template<class U>
        intrusive_ptr(intrusive_ptr<U> const& rhs, typename detail::sp_enable_if_convertible<U, T>::type = detail::sp_empty())
            : px(rhs.get()) {
            if (px != 0)
                intrusive_ptr_add_ref(px);
        }
        intrusive_ptr(intrusive_ptr const& rhs): px(rhs.px) {
            if (px != 0)
                intrusive_ptr_add_ref(px);
        }
        ~intrusive_ptr() {
            if (px != 0)
                intrusive_ptr_release(px);
        }
        template<class U> intrusive_ptr& operator=(intrusive_ptr<U> const& rhs) {
            this_type(rhs).swap(*this);
            return *this;
        }
        intrusive_ptr& operator=(intrusive_ptr const& rhs) {
            this_type(rhs).swap(*this);
            return *this;
        }
        intrusive_ptr& operator=(T* rhs) {
            this_type(rhs).swap(*this);
            return *this;
        }
        void reset() {
            this_type().swap(*this);
        }
        void reset(T* rhs) {
            this_type(rhs).swap(*this);
        }
        T* get() const {
            return px;
        }
        T& operator*() const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/intrusive_ptr.hpp" L"(" L"143" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return *px;
        }
        T* operator->() const {
            (void)((px != 0) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/intrusive_ptr.hpp" L"(" L"149" L"):\r\n" L"px != 0" L"\r\n"), __debugbreak(), 0));
            return px;
        }
        typedef T* this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const {
            return px == 0 ? 0 : &this_type::px;
        }
        bool operator!() const {
            return px == 0;
        }
        void swap(intrusive_ptr& rhs) {
            T* tmp = px;
            px = rhs.px;
            rhs.px = tmp;
        }
    private:
        T* px;
    };
    template<class T, class U> inline bool operator==(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) {
        return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) {
        return a.get() != b.get();
    }
    template<class T, class U> inline bool operator==(intrusive_ptr<T> const& a, U* b) {
        return a.get() == b;
    }
    template<class T, class U> inline bool operator!=(intrusive_ptr<T> const& a, U* b) {
        return a.get() != b;
    }
    template<class T, class U> inline bool operator==(T* a, intrusive_ptr<U> const& b) {
        return a == b.get();
    }
    template<class T, class U> inline bool operator!=(T* a, intrusive_ptr<U> const& b) {
        return a != b.get();
    }
    template<class T> inline bool operator<(intrusive_ptr<T> const& a, intrusive_ptr<T> const& b) {
        return std::less<T*>()(a.get(), b.get());
    }
    template<class T> void swap(intrusive_ptr<T> & lhs, intrusive_ptr<T> & rhs) {
        lhs.swap(rhs);
    }
    template<class T> T* get_pointer(intrusive_ptr<T> const& p) {
        return p.get();
    }
    template<class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p) {
        return static_cast<T*>(p.get());
    }
    template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p) {
        return const_cast<T*>(p.get());
    }
    template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p) {
        return dynamic_cast<T*>(p.get());
    }
    template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, intrusive_ptr<Y> const& p) {
        os << p.get();
        return os;
    }

#pragma warning(pop)

    template<class T> class enable_shared_from_this {
    protected:
        enable_shared_from_this() {
        }
        enable_shared_from_this(enable_shared_from_this const&) {
        }
        enable_shared_from_this& operator=(enable_shared_from_this const&) {
            return *this;
        }
        ~enable_shared_from_this() {
        }
    public:
        shared_ptr<T> shared_from_this() {
            shared_ptr<T> p(weak_this_);
            (void)((p.get() == this) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/enable_shared_from_this.hpp" L"(" L"50" L"):\r\n" L"p.get() == this" L"\r\n"), __debugbreak(), 0));
            return p;
        }
        shared_ptr<T const> shared_from_this() const {
            shared_ptr<T const> p(weak_this_);
            (void)((p.get() == this) || (OutputDebugStringW(L"\r\n*** ASSERTION FAILED in " L"../boost/smart_ptr/enable_shared_from_this.hpp" L"(" L"57" L"):\r\n" L"p.get() == this" L"\r\n"), __debugbreak(), 0));
            return p;
        }
    public:
        template<class X, class Y> void _internal_accept_owner(shared_ptr<X> const* ppx, Y* py) const {
            if (weak_this_.expired()) {
                weak_this_ = shared_ptr<T>(*ppx, py);
            }
        }
    private:
        mutable weak_ptr<T> weak_this_;
    };
}
