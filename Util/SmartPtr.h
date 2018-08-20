#ifndef __UTIL_SMARTPTR_H__
#define __UTIL_SMARTPTR_H__

#include <typeinfo>
#include <functional>
#include <memory>

namespace boost
{
    template<class T> inline void checked_delete(T * x)
    {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete x;
    }
    template<class T> inline void checked_array_delete(T * x)
    {
        typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
        (void) sizeof(type_must_be_complete);
        delete [] x;
    }
    template<class T> struct checked_deleter
    {
        typedef void result_type;
        typedef T * argument_type;
        void operator()(T * x) const
        {
            boost::checked_delete(x);
        }
    };
    template<class T> struct checked_array_deleter
    {
        typedef void result_type;
        typedef T * argument_type;
        void operator()(T * x) const
        {
            boost::checked_array_delete(x);
        }
    };
} 
namespace boost
{
    template<class E> inline void throw_exception(E const & e)
    {
        throw e;
    }
} 
namespace boost
{
    class bad_weak_ptr: public std::exception
    {
    public:
        virtual char const * what() const throw()
        {
            return "tr1::bad_weak_ptr";
        }
    };
} 
#ifndef ANKAI78802T
extern "C" long __cdecl _InterlockedIncrement( long volatile * );
extern "C" long __cdecl _InterlockedDecrement( long volatile * );
extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __cdecl _InterlockedExchange( long volatile *, long);
extern "C" long __cdecl _InterlockedExchangeAdd( long volatile *, long);
#pragma intrinsic( _InterlockedIncrement )
#pragma intrinsic( _InterlockedDecrement )
#pragma intrinsic( _InterlockedCompareExchange )
#pragma intrinsic( _InterlockedExchange )
#pragma intrinsic( _InterlockedExchangeAdd )
#endif

namespace boost
{
    namespace detail
    {
        class sp_counted_base
        {
        private:
            sp_counted_base( sp_counted_base const & );
            sp_counted_base & operator= ( sp_counted_base const & );
            long use_count_;        
            long weak_count_;       
        public:
            sp_counted_base(): use_count_( 1 ), weak_count_( 1 )
            {
            }
            virtual ~sp_counted_base() 
            {
            }
            virtual void dispose() = 0; 
            virtual void destroy() 
            {
                delete this;
            }
            virtual void * get_deleter( type_info const & ti ) = 0;
            void add_ref_copy()
            {
			#ifndef ANKAI78802T
                _InterlockedIncrement( &use_count_ );
			#else
				 InterlockedIncrement( &use_count_ );
			#endif
            }
            bool add_ref_lock() 
            {
                for ( ;; )
                {
                    long tmp = static_cast< long const volatile& >( use_count_ );
                    if ( tmp == 0 )
                        return false;
					#ifndef ANKAI78802T
                    if ( _InterlockedCompareExchange( &use_count_, tmp + 1, tmp ) == tmp )
					#else
					if ( InterlockedCompareExchange( &use_count_, tmp + 1, tmp ) == tmp )
					#endif
                        return true;
                }
            }
            void release() 
            {
			#ifndef ANKAI78802T
                if ( _InterlockedDecrement( &use_count_ ) == 0 )
			#else
				if ( InterlockedDecrement( &use_count_ ) == 0 )
			#endif
                {
                    dispose();
                    weak_release();
                }
            }
            void weak_add_ref() 
            {
				#ifndef ANKAI78802T
                _InterlockedIncrement( &weak_count_ );
				#else
				InterlockedIncrement( &weak_count_ );
				#endif
            }
            void weak_release() 
            {
			#ifndef ANKAI78802T
                if ( _InterlockedDecrement( &weak_count_ ) == 0 )
			#else
				if ( InterlockedDecrement( &weak_count_ ) == 0 )
			#endif
                {
                    destroy();
                }
            }
            long use_count() const 
            {
                return static_cast<long const volatile &>( use_count_ );
            }
        };
    } 
} 
namespace boost
{
    namespace detail
    {
        template<class X> class sp_counted_impl_p: public sp_counted_base
        {
        private:
            X * px_;
            sp_counted_impl_p( sp_counted_impl_p const & );
            sp_counted_impl_p & operator= ( sp_counted_impl_p const & );
            typedef sp_counted_impl_p<X> this_type;
        public:
            explicit sp_counted_impl_p( X * px ): px_( px )
            {
            }
            virtual void dispose() 
            {
                boost::checked_delete( px_ );
            }
            virtual void * get_deleter( type_info const & )
            {
                return 0;
            }
        };
        template<class P, class D> class sp_counted_impl_pd: public sp_counted_base
        {
        private:
            P ptr; 
            D del; 
            sp_counted_impl_pd( sp_counted_impl_pd const & );
            sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );
            typedef sp_counted_impl_pd<P, D> this_type;
        public:
            sp_counted_impl_pd( P p, D d ): ptr(p), del(d)
            {
            }
            virtual void dispose() 
            {
                del( ptr );
            }
            virtual void * get_deleter( type_info const & ti )
            {
                return ti == typeid(D)? &del: 0;
            }
        };
        template<class P, class D, class A> class sp_counted_impl_pda: public sp_counted_base
        {
        private:
            P p_; 
            D d_; 
            A a_; 
            sp_counted_impl_pda( sp_counted_impl_pda const & );
            sp_counted_impl_pda & operator= ( sp_counted_impl_pda const & );
            typedef sp_counted_impl_pda<P, D, A> this_type;
        public:
            sp_counted_impl_pda( P p, D d, A a ): p_( p ), d_( d ), a_( a )
            {
            }
            virtual void dispose() 
            {
                d_( p_ );
            }
            virtual void destroy() 
            {
                typedef typename A::template rebind< this_type >::other A2;
                A2 a2( a_ );
                this->~this_type();
                a2.deallocate( this, 1 );
            }
            virtual void * get_deleter( type_info const & ti )
            {
                return ti == typeid( D )? &d_: 0;
            }
        };
    } 
} 

namespace boost
{
    namespace detail
    {
        class weak_count;
        class shared_count
        {
        private:
            sp_counted_base * pi_;
            friend class weak_count;
        public:
            shared_count(): pi_(0) 
            {
            }
            template<class Y> explicit shared_count( Y * p ): pi_( 0 )
            {
                try
                {
                    pi_ = new sp_counted_impl_p<Y>( p );
                }
                catch (...)
                {
                    boost::checked_delete( p );
                    throw;
                }
            }
            template<class P, class D> shared_count(P p, D d): pi_(0)
            {
                try
                {
                    pi_ = new sp_counted_impl_pd<P, D>(p, d);
                }
                catch (...)
                {
                    d(p); 
                    throw;
                }
            }
            template<class P, class D, class A> shared_count( P p, D d, A a ): pi_( 0 )
            {
                typedef sp_counted_impl_pda<P, D, A> impl_type;
                typedef typename A::template rebind< impl_type >::other A2;
                A2 a2( a );
                try
                {
                    pi_ = a2.allocate( 1, static_cast< impl_type* >( 0 ) );
                    new( static_cast< void* >( pi_ ) ) impl_type( p, d, a );
                }
                catch (...)
                {
                    d( p );
                    if ( pi_ != 0 )
                    {
                        a2.deallocate( static_cast< impl_type* >( pi_ ), 1 );
                    }
                    throw;
                }
            }
            template<class Y>
            explicit shared_count( std::auto_ptr<Y> & r ): pi_( new sp_counted_impl_p<Y>( r.get() ) )
            {
                r.release();
            }
            ~shared_count() 
            {
                if ( pi_ != 0 )
                    pi_->release();
            }
            shared_count(shared_count const & r): pi_(r.pi_) 
            {
                if ( pi_ != 0 )
                    pi_->add_ref_copy();
            }
            explicit shared_count(weak_count const & r); 
            shared_count & operator= (shared_count const & r) 
            {
                sp_counted_base * tmp = r.pi_;
                if ( tmp != pi_ )
                {
                    if ( tmp != 0 )
                        tmp->add_ref_copy();
                    if ( pi_ != 0 )
                        pi_->release();
                    pi_ = tmp;
                }
                return *this;
            }
            void swap(shared_count & r) 
            {
                sp_counted_base * tmp = r.pi_;
                r.pi_ = pi_;
                pi_ = tmp;
            }
            long use_count() const 
            {
                return pi_ != 0? pi_->use_count(): 0;
            }
            bool unique() const 
            {
                return use_count() == 1;
            }
            friend inline bool operator==(shared_count const & a, shared_count const & b)
            {
                return a.pi_ == b.pi_;
            }
            friend inline bool operator<(shared_count const & a, shared_count const & b)
            {
                return std::less<sp_counted_base *>()( a.pi_, b.pi_ );
            }
            void * get_deleter(type_info const & ti) const
            {
                return pi_? pi_->get_deleter( ti ): 0;
            }
        };
        class weak_count
        {
        private:
            sp_counted_base * pi_;
            friend class shared_count;
        public:
            weak_count(): pi_(0) 
            {
            }
            weak_count(shared_count const & r): pi_(r.pi_) 
            {
                if (pi_ != 0)
                    pi_->weak_add_ref();
            }
            weak_count(weak_count const & r): pi_(r.pi_) 
            {
                if (pi_ != 0)
                    pi_->weak_add_ref();
            }
            ~weak_count() 
            {
                if (pi_ != 0) pi_->weak_release();
            }
            weak_count & operator= (shared_count const & r) 
            {
                sp_counted_base * tmp = r.pi_;
                if (tmp != 0)
                    tmp->weak_add_ref();
                if (pi_ != 0)
                    pi_->weak_release();
                pi_ = tmp;
                return *this;
            }
            weak_count & operator= (weak_count const & r) 
            {
                sp_counted_base * tmp = r.pi_;
                if (tmp != 0)
                    tmp->weak_add_ref();
                if (pi_ != 0)
                    pi_->weak_release();
                pi_ = tmp;
                return *this;
            }
            void swap(weak_count & r) 
            {
                sp_counted_base * tmp = r.pi_;
                r.pi_ = pi_;
                pi_ = tmp;
            }
            long use_count() const 
            {
                return pi_ != 0? pi_->use_count(): 0;
            }
            friend inline bool operator==(weak_count const & a, weak_count const & b)
            {
                return a.pi_ == b.pi_;
            }
            friend inline bool operator<(weak_count const & a, weak_count const & b)
            {
                return std::less<sp_counted_base *>()(a.pi_, b.pi_);
            }
        };
        inline shared_count::shared_count( weak_count const & r ): pi_( r.pi_ )
        {
            if ( pi_ == 0 || !pi_->add_ref_lock() )
            {
                boost::throw_exception( boost::bad_weak_ptr() );
            }
        }
    }
}
namespace boost
{
    template<class T> class weak_ptr;
    template<class T> class enable_shared_from_this;
    namespace detail
    {
        struct static_cast_tag
        {
        };
        struct const_cast_tag
        {
        };
        struct dynamic_cast_tag
        {
        };
        struct polymorphic_cast_tag
        {
        };
        template<class T> struct shared_ptr_traits
        {
            typedef T & reference;
        };
        template<> struct shared_ptr_traits<void>
        {
            typedef void reference;
        };
		/*
        template<> struct shared_ptr_traits<void const>
        {
            typedef void reference;
        };
        template<> struct shared_ptr_traits<void volatile>
        {
            typedef void reference;
        };
        template<> struct shared_ptr_traits<void const volatile>
        {
            typedef void reference;
        };
		*/
        template<class T, class Y> void sp_enable_shared_from_this( shared_count const & pn, boost::enable_shared_from_this<T> const * pe, Y const * px )
        {
            if (pe != 0)
                pe->_internal_weak_this._internal_assign(const_cast<Y*>(px), pn);
        }
        inline void sp_enable_shared_from_this( shared_count const & , ... )
        {
        }
		/*
        template< class T, class R > struct sp_enable_if_auto_ptr
        {
        };
        template< class T, class R > struct sp_enable_if_auto_ptr< std::auto_ptr< T >, R >
        {
            typedef R type;
        }; 
		*/
    } 
    template<class T> class shared_ptr
    {
    private:
        typedef shared_ptr<T> this_type;
    public:
        typedef T element_type;
        typedef T value_type;
        typedef T * pointer;
        typedef typename boost::detail::shared_ptr_traits<T>::reference reference;
        shared_ptr(): px(0), pn() 
        {
        }
        template<class Y>
        explicit shared_ptr( Y * p ): px( p ), pn( p ) 
        {
            boost::detail::sp_enable_shared_from_this( pn, p, p );
        }
        template<class Y, class D> shared_ptr(Y * p, D d): px(p), pn(p, d)
        {
            boost::detail::sp_enable_shared_from_this( pn, p, p );
        }
        template<class Y, class D, class A> shared_ptr( Y * p, D d, A a ): px( p ), pn( p, d, a )
        {
            boost::detail::sp_enable_shared_from_this( pn, p, p );
        }
        template<class Y>
        explicit shared_ptr(weak_ptr<Y> const & r): pn(r.pn) 
        {
            px = r.px;
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const & r): px(r.px), pn(r.pn) 
        {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const & r, boost::detail::static_cast_tag): px(static_cast<element_type *>(r.px)), pn(r.pn)
        {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const & r, boost::detail::const_cast_tag): px(const_cast<element_type *>(r.px)), pn(r.pn)
        {
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const & r, boost::detail::dynamic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
        {
            if (px == 0)
            {
                pn = boost::detail::shared_count();
            }
        }
        template<class Y>
        shared_ptr(shared_ptr<Y> const & r, boost::detail::polymorphic_cast_tag): px(dynamic_cast<element_type *>(r.px)), pn(r.pn)
        {
            if (px == 0)
            {
                boost::throw_exception(std::bad_cast());
            }
        }
        template<class Y>
        explicit shared_ptr(std::auto_ptr<Y> & r): px(r.get()), pn()
        {
            Y * tmp = r.get();
            pn = boost::detail::shared_count(r);
            boost::detail::sp_enable_shared_from_this( pn, tmp, tmp );
        }
		/*
        template<class Ap>
        explicit shared_ptr( Ap r, typename boost::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0 ): px( r.get() ), pn()
        {
            typename Ap::element_type * tmp = r.get();
            pn = boost::detail::shared_count( r );
            boost::detail::sp_enable_shared_from_this( pn, tmp, tmp );
        }
		*/
        template<class Y>
        shared_ptr & operator=(shared_ptr<Y> const & r) 
        {
            px = r.px;
            pn = r.pn; 
            return *this;
        }
        template<class Y>
        shared_ptr & operator=( std::auto_ptr<Y> & r )
        {
            this_type(r).swap(*this);
            return *this;
        }
		/*
        template<class Ap>
        typename boost::detail::sp_enable_if_auto_ptr< Ap, shared_ptr & >::type operator=( Ap r )
        {
            this_type( r ).swap( *this );
            return *this;
        }
		*/
        void reset() 
        {
            this_type().swap(*this);
        }
        template<class Y> void reset(Y * p) 
        {
            (void)( (!!(p == 0 || p != px)) || (_wassert(L"p == 0 || p != px", L"e:\\my documents\\my softwares\\boost_1_34_1\\boost\\shared_ptr.hpp", 293), 0) ); 
            this_type(p).swap(*this);
        }
        template<class Y, class D> void reset( Y * p, D d )
        {
            this_type( p, d ).swap( *this );
        }
        template<class Y, class D, class A> void reset( Y * p, D d, A a )
        {
            this_type( p, d, a ).swap( *this );
        }
        reference operator* () const 
        {
            //(void)( (!!(px != 0)) || (_wassert(L"px != 0", L"e:\\my documents\\my softwares\\boost_1_34_1\\boost\\shared_ptr.hpp", 309), 0) );
            return *px;
        }
        T * operator-> () const 
        {
            //(void)( (!!(px != 0)) || (_wassert(L"px != 0", L"e:\\my documents\\my softwares\\boost_1_34_1\\boost\\shared_ptr.hpp", 315), 0) );
            return px;
        }
        T * get() const 
        {
            return px;
        }
        typedef T * this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const 
        {
            return px == 0? 0: &this_type::px;
        }
        bool operator! () const 
        {
            return px == 0;
        }
        bool unique() const 
        {
            return pn.unique();
        }
        long use_count() const 
        {
            return pn.use_count();
        }
        void swap(shared_ptr<T> & other) 
        {
            std::swap(px, other.px);
            pn.swap(other.pn);
        }
        template<class Y> bool _internal_less(shared_ptr<Y> const & rhs) const
        {
            return pn < rhs.pn;
        }
        void * _internal_get_deleter(type_info const & ti) const
        {
            return pn.get_deleter(ti);
        }
    private:
        friend class shared_ptr;
        friend class weak_ptr;
        //template<class Y> friend class shared_ptr;
        //template<class Y> friend class weak_ptr;
        T * px;                     
        boost::detail::shared_count pn;    
    };  
    template<class T, class U> inline bool operator==(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a.get() != b.get();
    }
    template<class T, class U> inline bool operator<(shared_ptr<T> const & a, shared_ptr<U> const & b)
    {
        return a._internal_less(b);
    }
    template<class T> inline void swap(shared_ptr<T> & a, shared_ptr<T> & b)
    {
        a.swap(b);
    }
    template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::static_cast_tag());
    }
    template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::const_cast_tag());
    }
    template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_static_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::static_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_polymorphic_cast(shared_ptr<U> const & r)
    {
        return shared_ptr<T>(r, boost::detail::polymorphic_cast_tag());
    }
    template<class T, class U> shared_ptr<T> shared_polymorphic_downcast(shared_ptr<U> const & r)
    {
        (void)( (!!(dynamic_cast<T *>(r.get()) == r.get())) || (_wassert(L"dynamic_cast<T *>(r.get()) == r.get()", L"e:\\my documents\\my softwares\\boost_1_34_1\\boost\\shared_ptr.hpp", 484), 0) );
        return shared_static_cast<T>(r);
    }
    template<class T> inline T * get_pointer(shared_ptr<T> const & p)
    {
        return p.get();
    }
	/*
    template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, shared_ptr<Y> const & p)
    {
        os << p.get();
        return os;
    }
	*/
    template<class D, class T> D * get_deleter(shared_ptr<T> const & p)
    {
        return static_cast<D *>(p._internal_get_deleter(typeid(D)));
    }
} 

namespace boost
{
    template<class T> class enable_shared_from_this
    {
    protected:
        enable_shared_from_this()
        {
        }
        enable_shared_from_this(enable_shared_from_this const &)
        {
        }
        enable_shared_from_this & operator=(enable_shared_from_this const &)
        {
            return *this;
        }
        ~enable_shared_from_this()
        {
        }
    public:
        shared_ptr<T> shared_from_this()
        {
            shared_ptr<T> p(_internal_weak_this);
            BOOST_ASSERT(p.get() == this);
            return p;
        }
        shared_ptr<T const> shared_from_this() const
        {
            shared_ptr<T const> p(_internal_weak_this);
            BOOST_ASSERT(p.get() == this);
            return p;
        }
        typedef T _internal_element_type; // for bcc 5.5.1
        mutable weak_ptr<_internal_element_type> _internal_weak_this;
    };
} 

#endif //__UTIL_SMARTPTR_H__