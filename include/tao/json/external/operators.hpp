// The Art of C++ / Operators
// Copyright (c) 2013-2017 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/operators/

#ifndef TAOCPP_OPERATORS_INCLUDE_OPERATORS_HPP
#define TAOCPP_OPERATORS_INCLUDE_OPERATORS_HPP

#include <utility>

#ifndef TAOCPP_NO_RVALUE_REFERENCE_RESULTS
#define TAOCPP_OPERATORS_BASIC_OP( name, op )                           \
  template< typename T, typename U = T >                                \
  class name                                                            \
  {                                                                     \
    friend T operator op( const T& lhs, const U& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const T& lhs, U&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, const U& rhs )                     \
      noexcept( noexcept( lhs op##= rhs ) )                             \
    {                                                                   \
      lhs op##= rhs;                                                    \
      return std::move( lhs );                                          \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, U&& rhs )                          \
      noexcept( noexcept( lhs op##= std::move( rhs ) ) )                \
    {                                                                   \
      lhs op##= std::move( rhs );                                       \
      return std::move( lhs );                                          \
    }                                                                   \
  }
#else
#define TAOCPP_OPERATORS_BASIC_OP( name, op )                           \
  template< typename T, typename U = T >                                \
  class name                                                            \
  {                                                                     \
    friend T operator op( const T& lhs, const U& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const T& lhs, U&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, const U& rhs )                       \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, U&& rhs )                            \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
  }
#endif

#define TAOCPP_OPERATORS_BASIC_OP_LEFT( name, op )                      \
  template< typename T, typename U >                                    \
  class name##_left                                                     \
  {                                                                     \
    friend T operator op( const U& lhs, const T& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const U& lhs, T&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( U&& lhs, const T& rhs )                       \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( U&& lhs, T&& rhs )                            \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
  }

#ifndef TAOCPP_NO_RVALUE_REFERENCE_RESULTS
#define TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( name, op )               \
  template< typename T, typename U = T >                                \
  class commutative_##name                                              \
  {                                                                     \
    friend T operator op( const T& lhs, const U& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const T& lhs, U&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, const U& rhs )                     \
      noexcept( noexcept( lhs op##= rhs ) )                             \
    {                                                                   \
      lhs op##= rhs;                                                    \
      return std::move( lhs );                                          \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, U&& rhs )                          \
      noexcept( noexcept( lhs op##= std::move( rhs ) ) )                \
    {                                                                   \
      lhs op##= std::move( rhs );                                       \
      return std::move( lhs );                                          \
    }                                                                   \
                                                                        \
    friend T operator op( const U& lhs, const T& rhs )                  \
      noexcept( noexcept( T( rhs ), std::declval< T& >() op##= lhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( rhs );                                                     \
      nrv op##= lhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T&& operator op( const U& lhs, T&& rhs )                     \
      noexcept( noexcept( rhs op##= lhs ) )                             \
    {                                                                   \
      rhs op##= lhs;                                                    \
      return std::move( rhs );                                          \
    }                                                                   \
                                                                        \
    friend T operator op( U&& lhs, const T& rhs )                       \
      noexcept( noexcept( T( rhs ), std::declval< T& >() op##= std::move( lhs ) ) ) \
    {                                                                   \
      T nrv( rhs );                                                     \
      nrv op##= std::move( lhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T&& operator op( U&& lhs, T&& rhs )                          \
      noexcept( noexcept( rhs op##= std::move( lhs ) ) )                \
    {                                                                   \
      rhs op##= std::move( lhs );                                       \
      return std::move( rhs );                                          \
    }                                                                   \
  };                                                                    \
                                                                        \
  template< typename T >                                                \
  class commutative_##name< T >                                         \
  {                                                                     \
    friend T operator op( const T& lhs, const T& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T&& operator op( const T& lhs, T&& rhs )                     \
      noexcept( noexcept( rhs op##= lhs ) )                             \
    {                                                                   \
      rhs op##= lhs;                                                    \
      return std::move( rhs );                                          \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, const T& rhs )                     \
      noexcept( noexcept( lhs op##= rhs ) )                             \
    {                                                                   \
      lhs op##= rhs;                                                    \
      return std::move( lhs );                                          \
    }                                                                   \
                                                                        \
    friend T&& operator op( T&& lhs, T&& rhs )                          \
      noexcept( noexcept( lhs op##= std::move( rhs ) ) )                \
    {                                                                   \
      lhs op##= std::move( rhs );                                       \
      return std::move( lhs );                                          \
    }                                                                   \
  }
#else
#define TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( name, op )               \
  template< typename T, typename U = T >                                \
  class commutative_##name                                              \
  {                                                                     \
    friend T operator op( const T& lhs, const U& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const T& lhs, U&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, const U& rhs )                       \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, U&& rhs )                            \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const U& lhs, const T& rhs )                  \
      noexcept( noexcept( T( rhs ), std::declval< T& >() op##= lhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( rhs );                                                     \
      nrv op##= lhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const U& lhs, T&& rhs )                       \
      noexcept( noexcept( T( std::move( rhs ) ), std::declval< T& >() op##= lhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( rhs ) );                                        \
      nrv op##= lhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( U&& lhs, const T& rhs )                       \
      noexcept( noexcept( T( rhs ), std::declval< T& >() op##= std::move( lhs ) ) ) \
    {                                                                   \
      T nrv( rhs );                                                     \
      nrv op##= std::move( lhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( U&& lhs, T&& rhs )                            \
      noexcept( noexcept( T( std::move( rhs ) ), std::declval< T& >() op##= std::move( lhs ) ) ) \
    {                                                                   \
      T nrv( std::move( rhs ) );                                        \
      nrv op##= std::move( lhs );                                       \
      return nrv;                                                       \
    }                                                                   \
  };                                                                    \
                                                                        \
  template< typename T >                                                \
  class commutative_##name< T >                                         \
  {                                                                     \
    friend T operator op( const T& lhs, const T& rhs )                  \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( const T& lhs, T&& rhs )                       \
      noexcept( noexcept( T( lhs ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( lhs );                                                     \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, const T& rhs )                       \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= rhs, T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= rhs;                                                    \
      return nrv;                                                       \
    }                                                                   \
                                                                        \
    friend T operator op( T&& lhs, T&& rhs )                            \
      noexcept( noexcept( T( std::move( lhs ) ), std::declval< T& >() op##= std::move( rhs ), T( std::declval< T& >() ) ) ) \
    {                                                                   \
      T nrv( std::move( lhs ) );                                        \
      nrv op##= std::move( rhs );                                       \
      return nrv;                                                       \
    }                                                                   \
  }
#endif

namespace tao
{
  namespace operators
  {
    template< typename T, typename U = T >
    class equality_comparable
    {
      friend bool operator!=( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs == rhs ) ) )
      {
        return !static_cast< bool >( lhs == rhs );
      }

      friend bool operator==( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs == lhs ) ) )
      {
        return static_cast< bool >( rhs == lhs );
      }

      friend bool operator!=( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs != lhs ) ) )
      {
        return static_cast< bool >( rhs != lhs );
      }
    };

    template< typename T >
    class equality_comparable< T >
    {
      friend bool operator!=( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( lhs == rhs ) ) )
      {
        return !static_cast< bool >( lhs == rhs );
      }
    };

    template< typename T, typename U = T >
    class less_than_comparable
    {
      friend bool operator<=( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs > rhs ) ) )
      {
        return !static_cast< bool >( lhs > rhs );
      }

      friend bool operator>=( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ) ) )
      {
        return !static_cast< bool >( lhs < rhs );
      }

      friend bool operator<( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs > lhs ) ) )
      {
        return static_cast< bool >( rhs > lhs );
      }

      friend bool operator>( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
      {
        return static_cast< bool >( rhs < lhs );
      }

      friend bool operator<=( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs >= lhs ) ) )
      {
        return static_cast< bool >( rhs >= lhs );
      }

      friend bool operator>=( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs <= lhs ) ) )
      {
        return static_cast< bool >( rhs <= lhs );
      }
    };

    template< typename T >
    class less_than_comparable< T >
    {
      friend bool operator>( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
      {
        return static_cast< bool >( rhs < lhs );
      }

      friend bool operator<=( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
      {
        return !static_cast< bool >( rhs < lhs );
      }

      friend bool operator>=( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ) ) )
      {
        return !static_cast< bool >( lhs < rhs );
      }
    };

    template< typename T, typename U = T >
    class totally_ordered
      : less_than_comparable< T, U >, equality_comparable< T, U >
    {
    };

    template< typename T, typename U = T >
    class equivalent
    {
      friend bool operator==( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ), static_cast< bool >( lhs > rhs ) ) )
      {
        return !static_cast< bool >( lhs < rhs ) && !static_cast< bool >( lhs > rhs );
      }
    };

    template< typename T >
    class equivalent< T >
    {
      friend bool operator==( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ) ) )
      {
        return !static_cast< bool >( lhs < rhs ) && !static_cast< bool >( rhs < lhs );
      }
    };

    template< typename T, typename U = T >
    class partially_ordered
    {
      friend bool operator<=( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ), static_cast< bool >( lhs == rhs ) ) )
      {
        return static_cast< bool >( lhs < rhs ) || static_cast< bool >( lhs == rhs );
      }

      friend bool operator>=( const T& lhs, const U& rhs )
        noexcept( noexcept( static_cast< bool >( lhs > rhs ), static_cast< bool >( lhs == rhs ) ) )
      {
        return static_cast< bool >( lhs > rhs ) || static_cast< bool >( lhs == rhs );
      }

      friend bool operator<( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs > lhs ) ) )
      {
        return static_cast< bool >( rhs > lhs );
      }

      friend bool operator>( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
      {
        return static_cast< bool >( rhs < lhs );
      }

      friend bool operator<=( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs >= lhs ) ) )
      {
        return static_cast< bool >( rhs >= lhs );
      }

      friend bool operator>=( const U& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs <= lhs ) ) )
      {
        return static_cast< bool >( rhs <= lhs );
      }
    };

    template< typename T >
    class partially_ordered< T >
    {
      friend bool operator>( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
      {
        return static_cast< bool >( rhs < lhs );
      }

      friend bool operator<=( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( lhs < rhs ), static_cast< bool >( lhs == rhs ) ) )
      {
        return static_cast< bool >( lhs < rhs ) || static_cast< bool >( lhs == rhs );
      }

      friend bool operator>=( const T& lhs, const T& rhs )
        noexcept( noexcept( static_cast< bool >( rhs < lhs ), static_cast< bool >( lhs == rhs ) ) )
      {
        return static_cast< bool >( rhs < lhs ) || static_cast< bool >( lhs == rhs );
      }
    };

    TAOCPP_OPERATORS_BASIC_OP( addable, + );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( addable, + );
    TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( addable, + );

    TAOCPP_OPERATORS_BASIC_OP( subtractable, - );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( subtractable, - );

    TAOCPP_OPERATORS_BASIC_OP( multipliable, * );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( multipliable, * );
    TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( multipliable, * );

    TAOCPP_OPERATORS_BASIC_OP( dividable, / );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( dividable, / );

    TAOCPP_OPERATORS_BASIC_OP( modable, % );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( modable, % );

    template< typename T, typename U = T >
    class ring
      : commutative_addable< T, U >, subtractable< T, U >, subtractable_left< T, U >, multipliable< T, U >
    {
    };

    template< typename T >
    class ring< T >
      : commutative_addable< T >, subtractable< T >, multipliable< T >
    {
    };

    template< typename T, typename U = T >
    class ordered_ring
      : ring< T, U >, totally_ordered< T, U >
    {
    };

    template< typename T, typename U = T >
    class commutative_ring
      : commutative_addable< T, U >, subtractable< T, U >, subtractable_left< T, U >, commutative_multipliable< T, U >
    {
    };

    template< typename T >
    class commutative_ring< T >
      : commutative_addable< T >, subtractable< T >, commutative_multipliable< T >
    {
    };

    template< typename T, typename U = T >
    class ordered_commutative_ring
      : commutative_ring< T, U >, totally_ordered< T, U >
    {
    };

    template< typename T, typename U = T >
    class field
      : commutative_ring< T, U >, dividable< T, U >, dividable_left< T, U >
    {
    };

    template< typename T >
    class field< T >
      : commutative_ring< T >, dividable< T >
    {
    };

    template< typename T, typename U = T >
    class ordered_field
      : field< T, U >, totally_ordered< T, U >
    {
    };

    TAOCPP_OPERATORS_BASIC_OP( andable, & );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( andable, & );
    TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( andable, & );

    TAOCPP_OPERATORS_BASIC_OP( orable, | );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( orable, | );
    TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( orable, | );

    TAOCPP_OPERATORS_BASIC_OP( xorable, ^ );
    TAOCPP_OPERATORS_BASIC_OP_LEFT( xorable, ^ );
    TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE( xorable, ^ );

    template< typename T, typename U = T >
    class bitwise
      : andable< T, U >, orable< T, U >, xorable< T, U >
    {
    };

    template< typename T, typename U >
    class bitwise_left
      : andable_left< T, U >, orable_left< T, U >, xorable_left< T, U >
    {
    };

    template< typename T, typename U = T >
    class commutative_bitwise
      : commutative_andable< T, U >, commutative_orable< T, U >, commutative_xorable< T, U >
    {
    };

    TAOCPP_OPERATORS_BASIC_OP( left_shiftable, << );
    TAOCPP_OPERATORS_BASIC_OP( right_shiftable, >> );

    template< typename T, typename U = T >
    class shiftable
      : left_shiftable< T, U >, right_shiftable< T, U >
    {
    };

    template< typename T >
    class incrementable
    {
      friend T operator++( T& arg, int )
        noexcept( noexcept( T( arg ), ++arg, T( std::declval< T >() ) ) )
      {
        const T nrv( arg );
        ++arg;
        return nrv;
      }
    };

    template< typename T >
    class decrementable
    {
      friend T operator--( T& arg, int )
        noexcept( noexcept( T( arg ), --arg, T( std::declval< T >() ) ) )
      {
        const T nrv( arg );
        --arg;
        return nrv;
      }
    };

    template< typename T >
    class unit_steppable
      : incrementable< T >, decrementable< T >
    {
    };
  }
}

#undef TAOCPP_OPERATORS_BASIC_OP
#undef TAOCPP_OPERATORS_BASIC_OP_LEFT
#undef TAOCPP_OPERATORS_BASIC_OP_COMMUTATIVE

#endif
