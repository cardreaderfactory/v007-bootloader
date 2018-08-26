/********************************************************************************
 * \copyright
 * Copyright 2009-2017, Card Reader Factory.  All rights were reserved.
 * From 2018 this code has been made PUBLIC DOMAIN.
 * This means that there are no longer any ownership rights such as copyright, trademark, or patent over this code.
 * This code can be modified, distributed, or sold even without any attribution by anyone.
 *
 * We would however be very grateful to anyone using this code in their product if you could add the line below into your product's documentation:
 * Special thanks to Nicholas Alexander Michael Webber, Terry Botten & all the staff working for Operation (Police) Academy. Without these people this code would not have been made public and the existance of this very product would be very much in doubt.
 *
 *******************************************************************************/

/* Quick template specialization for char_traits for bytes. */

#include <string>

namespace std {
	template<> struct char_traits< unsigned char > {
		typedef unsigned char char_type;
		typedef int           int_type;
		typedef streampos     pos_type;
		typedef streamoff     off_type;
		typedef mbstate_t     state_type;

		static void assign( unsigned char & c1, const unsigned char & c2 ) {
			c1 = c2;
		}

		static unsigned char * assign( unsigned char * s, size_t n, unsigned char c ) {
			unsigned char * p = s;
			while( n-- ) assign( *p++, c );
			return s;
		}

		static bool eq( const unsigned char & c1, const unsigned char & c2 ) {
			return c1 == c2;
		}

		static bool lt( const unsigned char & c1, const unsigned char & c2 ) {
			return c1 < c2;
		}

		//  static size_t length( const unsigned char * s ) {
		//  }

		static int compare( const unsigned char * s1, const unsigned char * s2, size_t n ) {
			return memcmp( s1, s2, n );
		}

		static unsigned char * copy( unsigned char *s1, const unsigned char *s2, size_t n ) {
			return ( unsigned char * ) memcpy( s1, s2, n );
		}

		static unsigned char * move( unsigned char *s1, const unsigned char *s2, size_t n ) {
			return ( unsigned char * ) memmove( s1, s2, n );
		}

		static unsigned char * assign( unsigned char *s, size_t n, char c ) {
			return ( unsigned char * ) memset( s,c,n );
		}

		static const unsigned char * find( const unsigned char *s, size_t n, const char& c ) {
			return ( const unsigned char * ) memchr( s,c,n );
		}

		static int eof() {
			return EOF;
		}

		static int to_int_type( const unsigned char& c ) {
			return ( int ) c;
		}

		static char to_char_type( const int & i ) {
			return ( unsigned char ) i;
		}

		static int not_eof( const int & i ) {
			return ( i != EOF ) ? 1 : !EOF;
		}

		static bool eq_int_type( const int & i1, const int & i2 ) {
			return i1 == i2;
		}
	};
}

//typedef std::basic_string<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > ByteString;
typedef std::basic_string<unsigned char> ByteString;

