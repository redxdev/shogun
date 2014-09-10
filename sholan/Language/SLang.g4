grammar SLang;

@parser::header
{
	#pragma warning disable 3021

	using sholan.Compiler;
	using sholan.Compiler.Nodes;
	using sholan.Compiler.Nodes.Arguments;
}

@parser::members
{
	protected const int EOF = Eof;
}

@lexer::header
{
	#pragma warning disable 3021
}

@lexer::members
{
	protected const int EOF = Eof;
	protected const int HIDDEN = Hidden;
}

/*
 * Parser Rules
 */

compileUnit returns [ICompileNode rootNode]
	:	stms=statements EOF { $rootNode = $stms.tree; }
	;

statements returns [TreeNode tree]
	:
	{
		$tree = new TreeNode()
			{
				Children = new LinkedList<ICompileNode>()
			};
	}
	(
		stm=statement
		{
			$tree.Children.AddLast(
				$stm.node
				);
		}
	)*
	;

statement returns [ICompileNode node]
	:
	(
		s_if=stm_import_file { $node = $s_if.node; }
	|	s_ef=stm_extern_func { $node = $s_ef.node; }
	|	s_cf=stm_call_func { $node = $s_cf.node; }
	|	s_df=stm_define_func { $node = $s_df.node; }
	|	s_de=stm_define_entry { $node = $s_de.node; }
	|	s_re=stm_return { $node = $s_re.node; }
	)
	;

stm_import_file returns [ImportFileNode node]
	:	IMPORT fn=STRING
	{
		$node = new ImportFileNode()
			{
				Filepath = $fn.text.Substring(1, $fn.text.Length - 2)
			};
	}
	;

stm_extern_func returns [ExternalFunctionNode node]
	:
		EXTERN FUNCTION func=IDENT
	{
		$node = new ExternalFunctionNode()
			{
				SymbolName = $func.text,
				Line = $func.line
			};
	}
	;

stm_call_func returns [FunctionCallNode node]
	:
		func=IDENT
	{
		$node = new FunctionCallNode()
			{
				Function = $func.text,
				Arguments = new List<IArgument>()
			};
	}
		GROUP_START
	(
		arg1=atom { $node.Arguments.Add($arg1.arg); }
		(
			',' arg=atom { $node.Arguments.Add($arg.arg); }
		)*
	)?
		GROUP_END
	;

stm_define_func returns [InternalFunctionNode node]
	:	FUNCTION name=IDENT
	{
		$node = new InternalFunctionNode()
			{
				Function = $name.text,
				Arguments = new List<string>()
			};
	}
		GROUP_START
	(
		arg1=IDENT { $node.Arguments.Add($arg1.text); }
		(
			',' arg=IDENT { $node.Arguments.Add($arg.text); }
		)*
	)?
		GROUP_END
		BLOCK_START
		stms=statements { $node.Body = $stms.tree; }
		BLOCK_END
	;

stm_define_entry returns [EntryNode node]
	:	{ $node = new EntryNode(); }
		ENTRY
		BLOCK_START
		stms=statements { $node.Body = $stms.tree; }
		BLOCK_END
	;

stm_return returns [ReturnNode node]
	:	{ $node = new ReturnNode(); }
		RETURN
	(
		expr=atom { $node.Argument = $expr.arg; }
	)?
	;

atom returns [IArgument arg]
	:
		str=STRING { $arg = new ConstantArgument() { Value = $str.text }; }
	|	num=NUMBER { $arg = new ConstantArgument() { Value = $num.text }; }
	;

/*
 * Lexer Rules
 */

EXTERN
	:	'extern'
	;

FUNCTION
	:	'func'
	;

ENTRY
	:	'entry'
	;

RETURN
	:	'return'
	;

IMPORT
	:	'import'
	;

fragment ESCAPE_SEQUENCE
	:	'\\'
	(
		'\\'
	|	'"'
	|	'\''
	)
	;

STRING
	:
	(
		'"' ( ESCAPE_SEQUENCE | . )*? '"'
	|	'\'' ( ESCAPE_SEQUENCE | . )*? '\''
	)
	;

NUMBER
	:	'-'?
	(
		[0-9]* '.' [0-9]+
	|	[0-9]+
	)
	;

BLOCK_START
	:	'{'
	;

BLOCK_END
	:	'}'
	;

GROUP_START
	:	'('
	;

GROUP_END
	:	')'
	;

IDENT
	:	([a-zA-Z]) ([0-9a-zA-Z] | '_')*
	;

WS
	:	[ \n\t\r] -> channel(HIDDEN)
	;

COMMENT
	:	('//' ~[\r\n]*
	|	'/*' .*? '*/') -> channel(HIDDEN)
	;