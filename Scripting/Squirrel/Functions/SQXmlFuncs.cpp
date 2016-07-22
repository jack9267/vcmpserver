/*
	SQXmlFuncs.cpp
	This file is part of VCMP Server.

    VCMP Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VCMP Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VCMP Server.  If not, see <http://www.gnu.org/licenses/>.

	--------------------------------------------------------------------

    VC:MP Server 
		A server for VC:MP - A Multiplayer Modification For GTA:VC
    Copyright 2009 VRocker, Knight2k9 and Juppi

	File Author: Juppi

	--------------------------------------------------------------------

	Edited: 27/4/2009

	--------------------------------------------------------------------
*/

#include "SQXmlFuncs.h"


_SQUIRRELDEF( CSQXmlFuncs::CreateDoc )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* szName = NULL;
		sq_getstring( v, 2, &szName );

		TiXmlDocument* pDoc = new TiXmlDocument( (char*)szName );
		if ( pDoc )
		{
			push_XmlNode( (TiXmlNode*)pDoc );
			return 1;
		}
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::LoadDoc )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* szFile = NULL;
		sq_getstring( v, 2, &szFile );

		TiXmlDocument* pDoc = new TiXmlDocument();

		if ( pDoc )
		{
			if ( pDoc->LoadFile( szFile ) )
			{
				push_XmlNode( (TiXmlNode*)pDoc->FirstChildElement() );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::SaveDoc )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szFile = NULL;
		sq_getstring( v, 3, &szFile );

		TiXmlElement* pRoot = (TiXmlElement*)pPointer;

		if ( pRoot )
		{
			pRoot->ToDocument()->SaveFile( (char*)szFile );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::CreateElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szElement = NULL;
		sq_getstring( v, 3, &szElement );

		TiXmlNode* pParent = (TiXmlNode*)pPointer;

		if ( pParent )
		{
			TiXmlElement* pElement = new TiXmlElement( (char*)szElement );
			pParent->LinkEndChild( pElement );

			sq_pushuserpointer( v, pElement );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::RemoveElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			TiXmlNode* pParent = pElement->Parent();

			if ( pParent )
			{
				pParent->RemoveChild( pElement );

				sq_pushbool( v, true );
				return 1;
			}
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szElement = NULL;
		sq_getstring( v, 3, &szElement );

		TiXmlNode* pParent = (TiXmlNode*)pPointer;

		if ( pParent )
		{
			TiXmlElement* pElement = pParent->FirstChildElement( (char*)szElement );
			if ( pElement )
			{
				sq_pushuserpointer( v, pElement );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetParentElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			TiXmlNode* pParent = pElement->Parent();

			if ( pParent )
			{
				sq_pushuserpointer( v, (TiXmlElement*)pParent );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetNextSiblingElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			TiXmlNode* pSibling = pElement->NextSibling();

			if ( pSibling )
			{
				sq_pushuserpointer( v, (TiXmlElement*)pSibling );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetPrevSiblingElement )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			TiXmlNode* pSibling = pElement->PreviousSibling();

			if ( pSibling )
			{
				sq_pushuserpointer( v, (TiXmlElement*)pSibling );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetElementAttribute )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szAttribute = NULL;
		sq_getstring( v, 3, &szAttribute );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			if ( pElement->Attribute( (char*)szAttribute ) )
			{
				sq_pushstring( v, pElement->Attribute( (char*)szAttribute ), -1 );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::SetElementAttribute )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szAttribute = NULL;
		sq_getstring( v, 3, &szAttribute );

		const SQChar* szValue = NULL;
		sq_getstring( v, 4, &szValue );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			pElement->SetAttribute( (char*)szAttribute, (char*)szValue );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetElementName )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlNode* pElement = (TiXmlNode*)pPointer;
		if ( pElement )
		{
			char* szText = (char*)pElement->Value();

			sq_pushstring( v, szText, -1 );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::GetElementContent )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;
		if ( pElement )
		{
			char* szText = (char*)pElement->GetText();

			sq_pushstring( v, szText, -1 );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQXmlFuncs::SetElementContent )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szValue = NULL;
		sq_getstring( v, 3, &szValue );

		TiXmlElement* pElement = (TiXmlElement*)pPointer;

		if ( pElement )
		{
			/*

			TODO

			pElement->ToText()->SetValue( (char*)szValue );

			sq_pushbool( v, true );
			return 1;
			*/
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------




_IMPL_NATIVE_CONSTRUCTION( XmlNode, TiXmlNode );
_IMPL_NATIVE_CONSTRUCTION( XmlElement, TiXmlElement );
_IMPL_NATIVE_CONSTRUCTION_NODELETE( XmlDocument, TiXmlDocument );



_MEMBER_FUNCTION_IMPL( XmlNode, Save )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szFile = (const char*)sa.GetString( 2 );

	self->ToDocument()->SaveFile( szFile );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, AddChild )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szChild = (const char*)sa.GetString( 2 );

	TiXmlElement* pChild = new TiXmlElement( szChild );
	self->LinkEndChild( pChild );

	push_XmlNode( (TiXmlNode*)pChild );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, Remove )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	TiXmlNode* pParent = self->Parent();

	if ( pParent )
	{
		pParent->RemoveChild( self );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetChild )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szChild = (const char*)sa.GetString( 2 );

	TiXmlElement* pChild = self->FirstChildElement( szChild );

	if ( pChild )
	{
		push_XmlNode( (TiXmlNode*)pChild );
		return 1;
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetParent )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	TiXmlNode* pParent = self->Parent();

	if ( pParent )
	{
		push_XmlNode( pParent );
		return 1;
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetNextSibling )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	TiXmlNode* pSibling = self->NextSibling();

	if ( pSibling )
	{
		push_XmlNode( pSibling );
		return 1;
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetPrevSibling )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	TiXmlNode* pSibling = self->PreviousSibling();

	if ( pSibling )
	{
		push_XmlNode( pSibling );
		return 1;
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetAttribute )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szAttribute = (const char*)sa.GetString( 2 );

	TiXmlElement* pNode = (TiXmlElement*)self;

	if ( pNode )
	{
		const char* szValue = pNode->Attribute( szAttribute );
		if ( szValue )
		{
			sa.Return( szValue );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, SetAttribute )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szAttribute = (const char*)sa.GetString( 2 );

	const char* szValue = (const char*)sa.GetString( 3 );

	TiXmlElement* pNode = (TiXmlElement*)self;

	if ( pNode )
	{
		pNode->SetAttribute( szAttribute, szValue );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetName )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	sa.Return( self->Value() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, SetName )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szName = (const char*)sa.GetString( 2 );

	self->SetValue( szName );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, GetValue )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	TiXmlElement* pElement = (TiXmlElement*)self;

	if ( pElement )
	{
		sa.Return( pElement->GetText() );
		return 1;
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( XmlNode, SetValue )
{
	StackHandler sa(v);
	_CHECK_SELF( TiXmlNode, XmlNode );

	const char* szValue =  (const char*)sa.GetString( 2 );

	self->LinkEndChild( new TiXmlText( szValue ) ); 

	sa.Return( true );
	return 1;
}

//--------------------------------------------------



_BEGIN_CLASS( XmlNode )

//_MEMBER_FUNCTION( XmlNode, Save, 2, "xs" )
_MEMBER_FUNCTION( XmlNode, AddChild, 2, "xs" )
_MEMBER_FUNCTION( XmlNode, Remove, 1, "x" )
_MEMBER_FUNCTION( XmlNode, GetChild, 2, "xs" )
_MEMBER_FUNCTION( XmlNode, GetParent, 1, "x" )
_MEMBER_FUNCTION( XmlNode, GetNextSibling, 1, "x" )
_MEMBER_FUNCTION( XmlNode, GetPrevSibling, 1, "x" )
_MEMBER_FUNCTION( XmlNode, GetAttribute, 2, "xs" )
_MEMBER_FUNCTION( XmlNode, SetAttribute, 3, "xss" )
_MEMBER_FUNCTION( XmlNode, GetName, 1, "x" )
_MEMBER_FUNCTION( XmlNode, SetName, 2, "xs" )
_MEMBER_FUNCTION( XmlNode, GetValue, 1, "x" )
_MEMBER_FUNCTION( XmlNode, SetValue, 2, "xs" )

_END_CLASS( XmlNode )

_MEMBER_FUNCTION_IMPL( XmlElement, constructor )
{
	StackHandler sa( v );
	int iArgs = sa.GetParamCount();

	TiXmlElement* pElement = NULL;

	if ( iArgs == 2 )
	{
		const SQChar* szNode = sa.GetString( 2 );
		pElement = new TiXmlElement( (char*)szNode );
	}

	if ( pElement ) return construct_XmlElement( pElement );

	return sa.ThrowError( "XmlElement() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlElement, Clear )
{
	_CHECK_SELF( TiXmlElement, XmlElement );

	self->Clear();

	return 0;
}

_MEMBER_FUNCTION_IMPL( XmlElement, Attribute )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );

	int iArgs = sa.GetParamCount();
	if ( iArgs == 2 )
	{
		const SQChar* szName = sa.GetString( 2 );

		return sa.Return( self->Attribute( szName ) );
	}

	return sa.ThrowError( "XmlElement::Attribute() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlElement, SetAttribute )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );

	int iArgs = sa.GetParamCount();
	if ( iArgs == 3 )
	{
		const SQChar* szAttribute = sa.GetString( 2 );
		if ( sa.GetType( 3 ) == OT_STRING )
		{
			const SQChar* szValue = sa.GetString( 3 );

			self->SetAttribute( (char*)szAttribute, (char*)szValue );

			return sa.Return( true );
		}
		else if ( sa.GetType( 3 ) == OT_INTEGER )
		{
			int iValue = sa.GetInt( 3 );

			self->SetAttribute( (char*)szAttribute, iValue );

			return sa.Return( true );
		}
	}

	return sa.ThrowError( "XmlElement::SetAttribute() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlElement, Value )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );

	return sa.Return( self->Value() );
}

_MEMBER_FUNCTION_IMPL( XmlElement, GetText )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );

	return sa.Return( self->GetText() );
}

_MEMBER_FUNCTION_IMPL( XmlElement, SetText )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );

	int iArgs = sa.GetParamCount();

	if ( iArgs == 2 )
	{
		const SQChar* szValue = sa.GetString( 2 );
		TiXmlText* text = new TiXmlText( (char*)szValue );
		self->LinkEndChild( text );

		return sa.Return( true );
	}

	return sa.ThrowError( "XmlElement::SetText() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlElement, NextSiblingElement )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlElement, XmlElement );
	int iArgs = sa.GetParamCount();

	if ( iArgs == 1 )
	{
		TiXmlElement* xmlElement = self->NextSiblingElement();
		if ( xmlElement ) return push_XmlElement( xmlElement );
		else return false;
	}
	else if ( iArgs == 2 )
	{
		const SQChar* szValue = sa.GetString( 2 );

		TiXmlElement* xmlElement = self->NextSiblingElement( (char*)szValue );
		if ( xmlElement ) return push_XmlElement( xmlElement );
		else return false;
	}

	return sa.ThrowError( "XmlElement::NextSiblingElement() failed" );
}

_BEGIN_CLASS( XmlElement )
_MEMBER_FUNCTION( XmlElement, constructor, -1,"." )
_MEMBER_FUNCTION( XmlElement, Clear, 1, "x" )
_MEMBER_FUNCTION( XmlElement, Attribute, 2, "xs" )
_MEMBER_FUNCTION( XmlElement, SetAttribute, 3, "xss|n" )
_MEMBER_FUNCTION( XmlElement, Value, 1, "x" )
_MEMBER_FUNCTION( XmlElement, GetText, 1, "x" )
_MEMBER_FUNCTION( XmlElement, SetText, 2, "xs" )
_MEMBER_FUNCTION( XmlElement, NextSiblingElement, 0, 0 )
_END_CLASS( XmlElement )



_MEMBER_FUNCTION_IMPL( XmlDocument, constructor )
{
	StackHandler sa( v );
	int iArgs = sa.GetParamCount();

	TiXmlDocument* pDoc = NULL;

	if ( iArgs == 2 )
	{
		const SQChar* szFile = sa.GetString( 2 );
		pDoc = new TiXmlDocument( (char*)szFile );
	}
	else pDoc = new TiXmlDocument();

	if ( pDoc ) return construct_XmlDocument( pDoc );

	return sa.ThrowError( "XmlDocument() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlDocument, Clear )
{
	_CHECK_SELF( TiXmlDocument, XmlDocument );

	self->Clear();

	return 0;
}

_MEMBER_FUNCTION_IMPL( XmlDocument, LoadFile )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlDocument, XmlDocument );
	int iArgs = sa.GetParamCount();

	if ( iArgs == 1 ) return sa.Return( self->LoadFile() );
	else if ( iArgs == 2 )
	{
		const SQChar* szFile = sa.GetString( 2 );

		return sa.Return( self->LoadFile( (char*)szFile ) );
	}

	return sa.ThrowError( "XmlDocument::LoadFile() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlDocument, SaveFile )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlDocument, XmlDocument );
	int iArgs = sa.GetParamCount();

	if ( iArgs == 1 ) return sa.Return( self->SaveFile() );
	else if ( iArgs == 2 )
	{
		const SQChar* szFile = sa.GetString( 2 );

		return sa.Return( self->SaveFile( (char*)szFile ) );
	}

	return sa.ThrowError( "XmlDocument::SaveFile() failed" );
}

_MEMBER_FUNCTION_IMPL( XmlDocument, FirstChildElement )
{
	StackHandler sa( v );
	_CHECK_SELF( TiXmlDocument, XmlDocument );
	int iArgs = sa.GetParamCount();

	if ( iArgs == 1 )
	{
		push_XmlElement( (TiXmlElement*)self->FirstChildElement() );

		return 1;
	}
	else if ( iArgs == 2 )
	{
		const SQChar* szValue = sa.GetString( 2 );

		push_XmlElement( (TiXmlElement*)self->FirstChildElement( (char*)szValue ) );

		return 1;
	}

	return sa.ThrowError( "XmlDocument::FirstChildElement() failed" );
}

_BEGIN_CLASS( XmlDocument )

_MEMBER_FUNCTION( XmlDocument, constructor, -1,"." )
_MEMBER_FUNCTION( XmlDocument, Clear, 1, "x" )
_MEMBER_FUNCTION( XmlDocument, LoadFile, 0, 0 )
_MEMBER_FUNCTION( XmlDocument, SaveFile, 0, 0 )
_MEMBER_FUNCTION( XmlDocument, FirstChildElement, 0, 0 )

_END_CLASS( XmlDocument )
