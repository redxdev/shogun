grammar SLang;

@parser::header
{
	#pragma warning disable 3021

	using sholan.Compiler;
	using sholan.Compiler.Nodes;
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
	|	s_vd=stm_variable_def { $node = $s_vd.node; }
	|	s_as=stm_assembly { $node = $s_as.node; }
	|	s_ht=stm_halt { $node = $s_ht.node; }
	|	s_sv=stm_variable_set { $node = $s_sv.node; }
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
				Line = $func.line,
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
	;

stm_call_func returns [FunctionCallNode node]
	:
		func=IDENT
	{
		$node = new FunctionCallNode()
			{
				Function = $func.text,
				Arguments = new List<ICompileNode>()
			};
	}
		GROUP_START
	(
		arg1=expression { $node.Arguments.Add($arg1.node); }
		(
			',' arg=expression { $node.Arguments.Add($arg.node); }
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
		expr=expression { $node.Value = $expr.node; }
	)?
	;

stm_variable_def returns [DefineVariableNode node]
	:	VAR_DEF v=IDENT { $node = new DefineVariableNode() { VariableName = $v.text }; }
	(
		EQUAL
		expr=expression { $node.Value = $expr.node; }
	)?
	;

stm_assembly returns [RawAssemblyNode node]
	:	ASSEMBLY str=STRING_EXT { $node = new RawAssemblyNode() { Assembly = $str.text.Substring(2, $str.text.Length - 4) }; }
	;

stm_halt returns [HaltNode node]
	:	HALT { $node = new HaltNode(); }
	;

stm_variable_set returns [SetVariableNode node]
	:	var=IDENT EQUAL expr=expression { $node = new SetVariableNode() { VariableName = $var.text, Value = $expr.node }; }
	;

expression returns [ICompileNode node]
	:	a=atom { $node = $atom.node; }
	;

atom returns [ICompileNode node]
	:
		NIL { $node = new NilValueNode(); }
	|	str=STRING { $node = new ConstantNode() { Value = $str.text }; }
	|	num=NUMBER { $node = new ConstantNode() { Value = $num.text }; }
	|	B_TRUE { $node = new ConstantBoolNode() { Value = true }; }
	|	B_FALSE { $node = new ConstantBoolNode() { Value = false }; }
	|	var=IDENT { $node = new RetrieveVariableNode() { VariableName = $var.text }; }
	|	stm=statement { $node = $stm.node; $node.UseReturn = true; }
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

VAR_DEF
	:	'var'
	;

ASSEMBLY
	:	'asm'
	;

HALT
	:	'halt'
	;

EQUAL
	:	'='
	;

NIL
	:	'nil'
	;

B_TRUE
	:	'true'
	;

B_FALSE
	:	'false'
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

STRING_EXT
	: '[[' .*? ']]'
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