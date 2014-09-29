grammar SLang;

@parser::header
{
	#pragma warning disable 3021

	using System;
	using System.Diagnostics;
	using System.Threading;
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
	:	stms=statements EOF { $rootNode = $stms.node; }
	;

statements returns [TreeNode node]
	:
	{
		$node = new TreeNode()
			{
				Children = new LinkedList<ICompileNode>()
			};
	}
	(
		stm=statement
		{
			$node.Children.AddLast(
				$stm.node
				);
		}
	)*
	;

statement returns [ICompileNode node]
	:
	(
		stm_directive { $node = $stm_directive.node; }
	|	stm_import_file { $node = $stm_import_file.node; }
	|	stm_extern_func { $node = $stm_extern_func.node; }
	|	stm_call_func { $node = $stm_call_func.node; }
	|	stm_define_func { $node = $stm_define_func.node; }
	|	stm_define_entry { $node = $stm_define_entry.node; }
	|	stm_return { $node = $stm_return.node; }
	|	stm_break { $node = $stm_break.node; }
	|	stm_variable_def { $node = $stm_variable_def.node; }
	|	stm_assembly { $node = $stm_assembly.node; }
	|	stm_assembly_var { $node = $stm_assembly_var.node; }
	|	stm_halt { $node = $stm_halt.node; }
	|	stm_variable_set { $node = $stm_variable_set.node; }
	|	stm_if { $node = $stm_if.node; }
	|	stm_for_loop { $node = $stm_for_loop.node; }
	|	stm_while_loop { $node = $stm_while_loop.node; }
	)
	;

stm_directive returns [ICompileNode node]
	:
	(
		directive_debug_compiler { $node = new PlaceholderNode(); }
	|	directive_debug_break { $node = $directive_debug_break.node; }
	|	directive_compiler_break { $node = $directive_compiler_break.node; }
	|	directive_build_exports { $node = new BuildExportsNode(); }
	)
	;


directive_debug_compiler
	:	DIRECTIVE DEBUG_COMPILER { Console.WriteLine("Waiting for debugger..."); while(!Debugger.IsAttached) { Thread.Sleep(100); } Console.WriteLine("Debugger Attached"); }
	;

directive_debug_break returns [DebugBreakNode node]
	:	DIRECTIVE BREAK { $node = new DebugBreakNode(); }
	;

directive_compiler_break returns [CompilerBreakNode node]
	:	DIRECTIVE COMPILER_BREAK { $node = new CompilerBreakNode(); }
	;

directive_build_exports
	:	DIRECTIVE BUILD_EXPORTS
	;

stm_import_file returns [ImportFileNode node]
	:	IMPORT STRING
	{
		$node = new ImportFileNode()
			{
				Filepath = $STRING.text.Substring(1, $STRING.text.Length - 2),
				Mode = ImportMode.Inherit
			};
	}
	(
		EXPORT { $node.Mode = ImportMode.Export; }
	|	LIB	{ $node.Mode = ImportMode.Library; }
	)?
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
		IDENT
	{
		$node = new FunctionCallNode()
			{
				Function = $IDENT.text,
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
		stms=statements { $node.Body = $stms.node; }
		BLOCK_END
	;

stm_define_entry returns [InternalFunctionNode node]
	:	ENTRY
	{
		$node = new InternalFunctionNode()
			{
				Function = "+entry",
				Arguments = new List<string>()
			};
	}
		BLOCK_START
		stms=statements { $node.Body = $stms.node; }
		BLOCK_END
	;

stm_return returns [ReturnNode node]
	:	{ $node = new ReturnNode(); }
		RETURN
	(
		expression { $node.Value = $expression.node; }
	)?
	;

stm_break returns [BreakNode node]
	:	BREAK { $node = new BreakNode(); }
	;

stm_variable_def returns [DefineVariableNode node]
	:	VAR_DEF IDENT { $node = new DefineVariableNode() { VariableName = $IDENT.text }; }
	(
		EQUAL
		expression { $node.Value = $expression.node; }
	)?
	;

stm_assembly returns [RawAssemblyNode node]
	:	ASSEMBLY STRING_EXT { $node = new RawAssemblyNode() { Assembly = $STRING_EXT.text.Substring(2, $STRING_EXT.text.Length - 4) }; }
	|	ASSEMBLY VALUE STRING_EXT { $node = new RawAssemblyNode() { Assembly = $STRING_EXT.text.Substring(2, $STRING_EXT.text.Length - 4) }; $node.Attributes.Has("value"); }
	;

stm_assembly_var returns [RetrieveVariableNode node]
	:	ASSEMBLY VAR_DEF IDENT { $node = new RetrieveVariableNode() { VariableName = $IDENT.text }; }
	;

stm_halt returns [HaltNode node]
	:	HALT { $node = new HaltNode(); }
	;

stm_variable_set returns [SetVariableNode node]
	:	IDENT EQUAL expression { $node = new SetVariableNode() { VariableName = $IDENT.text, Value = $expression.node }; }
	;

stm_if returns [IfNode node]
	:	S_IF GROUP_START expression GROUP_END { $node = new IfNode() { Check = $expression.node }; }
	(
		stm=statement { $node.BranchTrue = $stm.node; }
	|	BLOCK_START
		stms=statements { $node.BranchTrue = $stms.node; }
		BLOCK_END
	)
	(
		S_ELSE
		(
			estm=statement { $node.BranchFalse = $estm.node; }
		|	BLOCK_START
			estms=statements { $node.BranchFalse = $estms.node; }
			BLOCK_END
		)
	)?
	;

stm_for_loop returns [ForLoopNode node]
	:	S_FOR GROUP_START init=statements SECTION check=expression SECTION incr=statements GROUP_END
		{
			$node = new ForLoopNode()
				{
					Init = $init.node,
					Check = $check.node,
					Increment = $incr.node
				};
		}

	(
		stm=statement { $node.Body = $stm.node; }
	|	BLOCK_START
		body=statements { $node.Body = $body.node; }
		BLOCK_END
	)
	;

stm_while_loop returns [WhileLoopNode node]
	:	S_WHILE GROUP_START check=expression GROUP_END
		{
			$node = new WhileLoopNode()
				{
					Check = $check.node
				};
		}
	(
		stm=statement { $node.Body = $stm.node; }
	|	BLOCK_START
		body=statements { $node.Body = $body.node; }
		BLOCK_END
	)
	;

expression returns [ICompileNode node]
	:	orExpr { $node = $orExpr.node; }
	;

orExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=andExpr { $node.Values.Add($a.node); }
	(
		OR OR b=andExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.OR); }
	)*
	;

andExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=equalityExpr { $node.Values.Add($a.node); }
	(
		AND AND b=equalityExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.AND); }
	)*
	;

equalityExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=concatExpr { $node.Values.Add($a.node); }
	(
		EQUAL EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.EQ); }
	|	EQUAL EQUAL EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.TEQ); }
	|	NOT EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.NEQ); }
	|	NOT EQUAL EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.NTEQ); }
	|	GREATER b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.GT); }
	|	GREATER EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.GTEQ); }
	|	LESS b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.LT); }
	|	LESS EQUAL b=concatExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.LTEQ); }
	)*
	;

concatExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=addExpr { $node.Values.Add($a.node); }
	(
		DOT DOT b=addExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.CONCAT); }
	)*
	;

addExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=mulExpr { $node.Values.Add($a.node); }
	(
		PLUS b=mulExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.ADD); }
	|	MINUS b=mulExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.SUB); }
	)*
	;

mulExpr returns [ExpressionNode node]
	:	{ $node = new ExpressionNode() { Values = new List<ICompileNode>(), Ops = new List<Opcode>() }; }
		a=notExpr { $node.Values.Add($a.node); }
	(
		MUL b=notExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.MUL); }
	|	DIV b=notExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.DIV); }
	|	MOD b=notExpr { $node.Values.Add($b.node); $node.Ops.Add(Opcode.MOD); }
	)*
	;

notExpr returns [ICompileNode node]
	:	NOT atom { $node = new NotNode() { Value = $atom.node }; }
	|	atom { $node = $atom.node; }
	;

atom returns [ICompileNode node]
	:
		NIL { $node = new NilValueNode(); }
	|	STRING { $node = new ConstantNode() { Value = $STRING.text }; }
	|	NUMBER { $node = new ConstantNode() { Value = $NUMBER.text }; }
	|	B_TRUE { $node = new ConstantBoolNode() { Value = true }; }
	|	B_FALSE { $node = new ConstantBoolNode() { Value = false }; }
	|	IDENT { $node = new RetrieveVariableNode() { VariableName = $IDENT.text }; }
	|	GROUP_START expression GROUP_END { $node = $expression.node; }
	|	statement { $node = $statement.node; $node.UseReturn = true; }
	;

/*
 * Lexer Rules
 */

EXTERN
	:	'extern'
	;

EXPORT
	:	'export'
	;

LIB
	:	'lib'
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

BREAK
	:	'break'
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

VALUE
	:	'value'
	;

HALT
	:	'halt'
	;

EQUAL
	:	'='
	;

NOT
	:	'!'
	;

GREATER
	:	'>'
	;

LESS
	:	'<'
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

S_IF
	:	'if'
	;

S_ELSE
	:	'else'
	;

S_FOR
	:	'for'
	;

S_WHILE
	:	'while'
	;

SECTION
	:	';'
	;

SPEC
	:	':'
	;

PLUS
	:	'+'
	;

MINUS
	:	'-'
	;

MUL
	:	'*'
	;

DIV
	:	'/'
	;

MOD
	:	'%'
	;

AND
	:	'&'
	;

OR
	:	'|'
	;

DOT
	:	'.'
	;

DIRECTIVE
	:	'#'
	;

DEBUG_COMPILER
	:	'debug-compiler'
	;

COMPILER_BREAK
	:	'cbreak'
	;

BUILD_EXPORTS
	:	'build-exports'
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