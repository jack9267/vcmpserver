#include "SquirrelMain.h"

_IMPL_NATIVE_CONSTRUCTION( Vector, Vector );

_MEMBER_FUNCTION_IMPL( Vector, constructor )
{
	Vector temp;
	Vector *newv = NULL;
	StackHandler sa(v);
	int nparams = sa.GetParamCount();
	switch(nparams) {
	case 1:
		temp.x = 0; 
		temp.y = 0;
		temp.z = 0;
		break;
	case 2:
		if(sa.GetType(2) == OT_INSTANCE) {
			_CHECK_INST_PARAM( vec, 2, Vector, Vector );
			if(vec)	temp = *vec;
			else return sa.ThrowError(_T("Vector() invalid instance type"));
		}
		break;
	case 4:
		temp.x = sa.GetFloat(2);
		temp.y = sa.GetFloat(3);
		temp.z = sa.GetFloat(4);
		break;
	default:
		return sa.ThrowError(_T("Vector() wrong parameters"));
	}
	newv = new Vector( temp );
	return construct_Vector( newv );
}

_MEMBER_FUNCTION_IMPL( Vector, _set )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );
	const SQChar *s = sa.GetString(2);
	int index = s?s[0]:sa.GetInt(2);
	switch(index) {
	case 0: case 'x':
		return sa.Return( self->x = sa.GetFloat(3) );
		break;
	case 1: case 'y':
		return sa.Return( self->y = sa.GetFloat(3) );
		break;
	case 2: case 'z':
		return sa.Return( self->z = sa.GetFloat(3) );
		break;
	}
	
	return SQ_ERROR;
}

_MEMBER_FUNCTION_IMPL( Vector, _get )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );
	const SQChar *s = sa.GetString(2);
	if(s && (s[1] != 0))
		return SQ_ERROR;
	int index = s && (s[1] == 0)?s[0]:sa.GetInt(2);
	switch(index) {
		case 0: case 'x': return sa.Return(self->x); break;
		case 1: case 'y': return sa.Return(self->y); break;
		case 2: case 'z': return sa.Return(self->z); break;
	}
	return SQ_ERROR;
}

_MEMBER_FUNCTION_IMPL( Vector, _tostring )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );

	char szTemp[ 128 ] = { 0 };

	sprintf( szTemp, "%.2f %.2f %.2f", self->x, self->y, self->z );

	return sa.Return( szTemp );
}

_MEMBER_FUNCTION_IMPL( Vector, _cmp )
{
	
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );
	_CHECK_INST_PARAM( vec, 2, Vector, Vector );
	if( (*self) == (*vec) ) return sa.Return( 0 );
	if( (*self) < (*vec) ) return sa.Return( -1 );
	return sa.Return( 1 );
}

_MEMBER_FUNCTION_IMPL( Vector, _add )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );
	_CHECK_INST_PARAM( vec, 2, Vector, Vector );

	Vector* vecRet = new Vector( (*self) + (*vec) );
	return sa.Return( new_Vector( vecRet ) );
}

_MEMBER_FUNCTION_IMPL( Vector, _sub )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );
	_CHECK_INST_PARAM( vec, 2, Vector, Vector );

	Vector* vecRet = new Vector( (*self) - (*vec) );
	return sa.Return( new_Vector( vecRet ) );
}

_MEMBER_FUNCTION_IMPL( Vector, _mul )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector,Vector );

	Vector* vecRet = new Vector( (*self) * sa.GetFloat(2) );
	return sa.Return( new_Vector( vecRet ) );
}

_MEMBER_FUNCTION_IMPL( Vector, _div )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector,Vector );
	Vector* vecRet = new Vector( (*self) / sa.GetFloat(2) );
	return sa.Return( new_Vector( vecRet ) );
}

_MEMBER_FUNCTION_IMPL( Vector, IsZero )
{
	StackHandler sa(v);
	_CHECK_SELF( Vector, Vector );

	return sa.Return( self->IsZero() );
}


_BEGIN_CLASS( Vector )
_MEMBER_FUNCTION( Vector, constructor, -1, ".n|xnn" )
_MEMBER_FUNCTION( Vector, _set, 3, "xs|n" )
_MEMBER_FUNCTION( Vector, _get, 2, "xs|n" )
_MEMBER_FUNCTION( Vector, _tostring, 1, "x" )
_MEMBER_FUNCTION( Vector, _add, 2, "xx" )
_MEMBER_FUNCTION( Vector, _sub, 2, "xx" )
_MEMBER_FUNCTION( Vector, _mul, 2, "xn" )
_MEMBER_FUNCTION( Vector, _div, 2, "xn" )
_MEMBER_FUNCTION( Vector, _cmp, 2, "xx" )
_MEMBER_FUNCTION( Vector, IsZero, 1, "x" )
_END_CLASS( Vector )