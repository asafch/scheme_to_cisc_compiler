#use "pc.ml";;

exception X_not_yet_implemented;;
exception X_this_should_not_happen;;

let rec ormap f s =
  match s with
  | [] -> false
  | car :: cdr -> (f car) || (ormap f cdr);;

let rec andmap f s =
  match s with
  | [] -> true
  | car :: cdr -> (f car) && (andmap f cdr);;

let string_to_list str =
  let rec loop i limit =
    if i = limit then []
    else (String.get str i) :: (loop (i + 1) limit)
  in
  loop 0 (String.length str);;

let list_to_string s =
  let rec loop s n =
    match s with
    | [] -> String.make n '?'
    | car :: cdr ->
       let result = loop cdr (n + 1) in
       String.set result n car;
       result
  in
  loop s 0;;

type fraction = {numerator : int; denominator : int};;

type number =
  | Int of int
  | Fraction of fraction;;

type sexpr =
  | Void
  | Bool of bool
  | Nil
  | Number of number
  | Char of char
  | String of string
  | Symbol of string
  | Pair of sexpr * sexpr
  | Vector of sexpr list;;

module type SEXPR = sig
  val sexpr_to_string : sexpr -> string
end;; (* signature SEXPR *)

module Sexpr : SEXPR = struct

exception X_invalid_fraction of fraction;;

let normalize_scheme_symbol str =
  let s = string_to_list str in
  if (andmap
  (fun ch -> (ch = (Char.lowercase ch)))
  s) then str
  else Printf.sprintf "|%s|" str;;

let number_to_string num =
  match num with
    | Int i-> string_of_int i
    | Fraction {numerator = a; denominator = b} ->
        (string_of_int a) ^ "/" ^ (string_of_int b);;

let sexpr_to_string sexpr =
  let rec make_sexpr_to_string sexpr =
    let rec pair_to_string exp =
      match exp with
      | (ex1, Nil) -> make_sexpr_to_string(ex1)
      | (ex1, Pair(p1,p2)) -> (make_sexpr_to_string ex1) ^ " " ^ (pair_to_string (p1,p2))
      | (ex1, ex2) -> (make_sexpr_to_string ex1) ^ "." ^ (make_sexpr_to_string ex2)
    in
    match sexpr with
    | Void -> ""
    | Bool b -> if b then "#t" else "#f"
    | Nil -> "'()"
    | Number num -> (number_to_string num)
    | Char c when c = '\n' -> "#\\newline"
    | Char c when c = '\r' -> "#\\return"
    | Char c when c = '\t' -> "#\\tab"
    | Char c when c = ' ' -> "#\\space"
    | Char c when c = Char.chr 12 -> "#\\page"
    | Char c -> "#\\" ^ (Char.escaped c)
    | String s -> "\""^ s ^"\""
    | Symbol "quote" -> ""
    | Symbol sym -> sym
    | Pair (Symbol "quote", Pair (Symbol sym, Nil)) -> "'" ^ sym
    | Pair (p1,p2) -> "(" ^ (pair_to_string (p1,p2)) ^ ")"
    | Vector exprs -> "#(" ^
        (List.fold_right
              (fun expr exprs -> exprs ^ " " ^(make_sexpr_to_string expr)) exprs "") ^
        ")" in
  make_sexpr_to_string sexpr;;

end;; (* struct Sexpr *)

module type PARSER = sig
  val read_sexpr : string -> sexpr
  val read_sexprs : string -> sexpr list
end;;

module Parser : PARSER = struct

open PC;;

let make_char_value base_char displacement =
  let base_char_value = Char.code base_char in
  fun ch -> (Char.code ch) - base_char_value + displacement;;
  let nt_digit_0_9 = pack (range '0' '9') (make_char_value '0' 0);;

let nt_digit_1_9 = pack (range '1' '9') (make_char_value '0' 0);;
(* NOTE: we can't use rane_ci since A and a have diffrent
	ASCII values and mack_char_value can't be applied in that way *)
let nt_a_to_f =   pack (range 'a' 'f') (make_char_value 'a' 10);;

let nt_A_to_F =   pack (range 'A' 'F') (make_char_value 'A' 10);;

let nt_aA_to_fF = disj nt_a_to_f nt_A_to_F;;

let nt_hexa_digit = disj nt_digit_0_9 nt_aA_to_fF;;

let nt_int_dec =
  let nt = char '-' in
  let nt = pack nt (fun e -> -1) in
  let nt' = char '+' in
  let nt' = pack nt' (fun e -> 1) in
  let nt = disj nt nt' in
  let nt = maybe nt in
  let nt = pack nt (function | None -> 1 | Some(mult) -> mult) in
  let nt' = range '0' '9' in
  let nt' = pack nt' (make_char_value '0' 0) in
  let nt' = plus nt' in
  let nt' = pack nt' (fun s -> List.fold_left (fun a b -> a * 10 + b) 0 s) in
  let nt = caten nt nt' in
  let nt = pack nt (fun (mult, n) -> (mult * n)) in
  nt;;

  let nt_int_hex =
  let nt = char '-' in
  let nt = pack nt (fun e -> -1) in
  let nt' = char '+' in
  let nt' = pack nt' (fun e -> 1) in
  let nt = disj nt nt' in
  let nt' = word_ci "0x" in
  let nt' = pack (caten nt' (plus nt_hexa_digit)) (fun (preffix,num)-> num) in
  let nt = maybe nt in
  let nt = pack nt (function | None -> 1 | Some(mult) -> mult) in
  let nt' = pack nt' (fun s -> List.fold_left (fun a b -> a * 16 + b) 0 s) in
  let nt = caten nt nt' in
  let nt = pack nt (fun (mult, n) -> (mult * n)) in
  nt;;


let nt_int = disj nt_int_hex nt_int_dec;;

let rec gcd m n =
  if n = 0 then
    m
  else
    gcd n (m mod n);;

exception X_variable_not_bound of string;;
exception X_cannot_represent_type of string;;

let handle_fraction numerator denominator =
  let divisor = gcd numerator denominator in
  let numerator = numerator / divisor in
  let denominator = denominator / divisor in
  if denominator = 1 then
    (Number (Int numerator))
  else if denominator > 0 then
    (Number (Fraction {numerator; denominator}))
  else
    let numerator = -numerator in
    let denominator = -denominator in
  (Number (Fraction {numerator; denominator}));;

let nt_fraction =
  pack (caten nt_int (caten (char '/') nt_int))
        (fun (numerator, (delimiter, denominator)) ->
              if denominator = 0 then
                raise (X_cannot_represent_type
                		((string_of_int numerator)^ "/" ^ (string_of_int denominator)))
              else if denominator < 0 then
              	raise (X_variable_not_bound (string_of_int denominator))
              else
                handle_fraction numerator denominator);;

(* ' '=0x20; '~' = 0x7D *)
let nt_allowed_chars = range ' ' '~' ;;
let nt_all_chars = range (Char.chr 0) (Char.chr 127)

let nt_char =
  let nt_char_perfix = word("#\\") in
  let nt_char =
    disj_list[ (pack (word_ci "newline")  (fun _-> Char '\n') );
			   (pack (word_ci "space")    (fun _-> Char ' ') );
               (pack (word_ci "return")   (fun _-> Char '\r') );
               (pack (word_ci "tab")      (fun _-> Char '\t') );
               (pack (word_ci "page")     (fun _-> Char (Char.chr 12) ));
               (pack nt_all_chars         (fun c-> Char c) )] in
  pack (caten nt_char_perfix nt_char)
        (fun (_,value) -> value);;

let nt_string_char =
    (disj_list[ (pack (word_ci "\\n")  (fun _-> '\n') );
               (pack (word_ci "\\r")  (fun _-> '\r') );
               (pack (word_ci "\\t")  (fun _-> '\t') );
               (pack (word_ci "\\f")  (fun _-> (Char.chr 12)) );
               (pack (word_ci "\\\\") (fun _-> '\\') );
               (pack (word_ci "\\\"") (fun _-> '\"') );
               (diff nt_all_chars (char '\"'))]);;

let nt_new_line = char (Char.chr 10);;

let nt_line_comment =
    let nt = diff (range (Char.chr 0) (Char.chr 127)) (char '\n') in
    let nt = caten (char ';')
        (caten (star nt)
                (disj (pack nt_end_of_input (fun _->'\n'))
                      nt_new_line)) in
    pack nt
      (* m stands for moshe `) *)
      (fun _ -> 'm');;

let nt_sexpr =
    let rec make_rec_sexpr () =
    	(* Recursive sexpr *)
        let nt_sexpr = (delayed make_rec_sexpr) in
        (* #; comments *)
	    let nt_sexpr_comment = pack (caten (word "#;")
			(caten (star nt_whitespace) nt_sexpr))
      		(* m stands for moshe `) *)
		    (fun _ -> 'm') in
		(* white space and noise *)
        let nt_whitespace = (disj_list[nt_whitespace; nt_line_comment; nt_sexpr_comment; nt_new_line;]) in
		(* Recursive-space-free sexpr *)
		let nt_sexpr = pack (caten (star nt_whitespace)  nt_sexpr)
		 	(fun (_,sexpr)->sexpr) in
        let nt_sexpr = pack (caten nt_sexpr (star nt_whitespace))
         	(fun (sexpr,_)->sexpr) in
        (* Bool. *)
        let nt_false = pack (word_ci "#f")
        	(fun _ -> Bool false) in
        let nt_true =  pack (word_ci "#t")
        	(fun _ -> Bool true) in
        let nt_boolean = disj nt_true nt_false in
        (* Numbers. *)
        let nt_number = disj  nt_fraction
	        (pack nt_int (fun (i)-> Number (Int i))) in
        (*  Symbol. *)
        let nt_symbol =
          let nt_letters = range_ci 'a' 'z' in
          let nt_nums = range '0' '9' in
          let nt_punc = disj_list[ (char '!');
	          (char '$');
	          (char '^');
	          (char '*');
	          (char '-');
	          (char '_');
	          (char '=');
	          (char '+');
	          (char '<');
	          (char '>');
	          (char '/');
	          (char '?');] in
          pack (plus (disj_list[nt_letters;
            nt_nums;
            nt_punc;]))
          (* Try to match the symbol with a number/hexa number
          	if indeed successfull return a number otherwise
          	return a symbol *)
          (fun char_lst ->
          	try let (num_ex,s) = (nt_number char_lst) in
	          	match s with
	          	| [] -> num_ex
	          	| _  -> raise X_no_match
	        with X_no_match -> Symbol (String.lowercase (list_to_string char_lst))) in

        (* String. *)
        let nt_string = pack
                              (caten
                                (pack (caten (char '\"') (star nt_string_char))
                                  (fun (_,chars) -> list_to_string(chars)))
                                (char '\"'))
                              (fun (str, _) -> String str) in

        let nt_left_paren =  pack (caten (star nt_whitespace) (char '(')) (fun (_,paren)->paren) in
        let nt_left_paren =  pack (caten  nt_left_paren (star nt_whitespace)) (fun (paren,_)->paren) in
        let nt_right_paren =  pack (caten (char ')') (star nt_whitespace))  (fun (paren,_)->paren) in
        let nt_right_paren =  pack (caten (star nt_whitespace) nt_right_paren)  (fun (_,paren)->paren) in
        (* Nil. *)
        let nt_nil = pack (caten nt_left_paren (caten (star nt_whitespace) nt_right_paren)) (fun _-> Nil) in
        (* Pair / List *)
        let nt_proper_list_items = star nt_sexpr in
        let nt_proper_list = pack (caten nt_left_paren
                                          (pack (caten  nt_proper_list_items nt_right_paren)
                                                (fun (sexprs, right_paren) -> sexprs)))
                            (fun (left_paren, sexprs) -> sexprs) in
        let nt_proper_list =
           pack nt_proper_list
                (fun (sexprs) ->
                  List.fold_right (fun expr exprs -> Pair (expr,exprs)) sexprs Nil) in

        let nt_dot = pack (caten  (star nt_whitespace)
                                  (pack (caten (char '.') (star nt_whitespace) )
                                    (fun (dot,_) -> dot)))
                    (fun (_,dot) -> dot) in

        let nt_improper_list = pack (caten (pack  (caten (plus nt_sexpr) nt_dot)
                                                  (fun (exprs, dot) -> exprs))
                                    nt_sexpr)
                                (fun (sexprs, sexpr) -> List.fold_right (fun expr exprs -> Pair (expr,exprs)) sexprs sexpr) in
        let nt_improper_list = pack (caten nt_left_paren
                                          (pack (caten  nt_improper_list nt_right_paren)
                                                (fun (sexprs, right_paren) -> sexprs)))
                            (fun (left_paren, sexprs) -> sexprs) in
        let nt_pair = disj nt_improper_list nt_proper_list in
        (* Vector. *)
        let nt_vector = char '#' in
        let nt_vector = pack (caten (star nt_whitespace) nt_vector) (fun (junk,delim) -> delim) in
        let nt_vector = caten nt_vector
                          (pack (caten nt_left_paren
                                      (pack (caten nt_proper_list_items nt_right_paren)
                                      (fun (sexprs, right_paren) -> sexprs)))
                          (fun (left_paren, sexprs) -> sexprs)) in
        let nt_vector = (pack nt_vector (fun (_, sexprs) -> Vector sexprs)) in
        (* Quote And Friends.  *)
        let nt_quote_forms =
          let nt_quote = pack
                          (caten (char '\'') nt_sexpr)
                          (fun (q_var,sexpr) -> Pair( Symbol "quote", Pair(sexpr, Nil))) in
          let nt_qquote = pack
                          (caten (char '`') nt_sexpr)
                          (fun (q_var,sexpr) -> Pair( Symbol "quasiquote", Pair(sexpr, Nil))) in
          let nt_unquote_sl = pack
                          (caten (word ",@") nt_sexpr)
                          (fun (q_var,sexpr) -> Pair( Symbol "unquote-splicing", Pair(sexpr, Nil))) in
          let nt_unquote = pack
                        (caten (char ',') nt_sexpr)
                        (fun (q_var,sexpr) -> Pair( Symbol "unquote", Pair(sexpr, Nil))) in
          disj_list[nt_quote;
                    nt_qquote;
                    nt_unquote_sl;
                    nt_unquote;] in
        (* Clear Whitespace. *)
        let nt_boolean = pack (caten
                    (pack (caten (star nt_whitespace) nt_boolean) (fun (_,ex)-> ex))
                      (star nt_whitespace))
                  (fun (ex,_) -> ex) in
        let nt_char = pack (caten
                            (pack (caten (star nt_whitespace) nt_char) (fun (_,sexpr)-> sexpr))
                            (star nt_whitespace))
                        (fun (sexpr,_) -> sexpr) in
        let nt_string = pack (caten
                            (pack (caten (star nt_whitespace) nt_string) (fun (_,sexpr)-> sexpr))
                            (star nt_whitespace))
                        (fun (sexpr,_) -> sexpr) in
        let nt_quote_forms = pack (caten
                            (pack (caten (star nt_whitespace) nt_quote_forms) (fun (_,sexpr)-> sexpr))
                            (star nt_whitespace))
                        (fun (sexpr,_) -> sexpr) in
        let nt_symbol = pack (caten
                        (pack (caten (star nt_whitespace) nt_symbol) (fun (_,sexpr)-> sexpr))
                        (star nt_whitespace))
                    (fun (sexpr,_) -> sexpr) in
      (disj_list[
              nt_boolean;
              nt_char;
              nt_symbol;
              nt_nil;
              nt_string;
              nt_pair;
              nt_vector;
              nt_quote_forms;]) in
    make_rec_sexpr();;

let read_sexpr string =
     match (nt_sexpr (string_to_list string)) with
     | (e,[]) -> e
     | _ -> raise PC.X_no_match;;

let read_sexprs string =
     match ((star nt_sexpr (string_to_list string)))  with
     | (e,[]) -> e
     | _ -> raise PC.X_no_match;;

end;; (* struct Parser *)

(* work on the tag parser starts here *)


type expr =
  | Const of sexpr
  | Var of string
  | If of expr * expr * expr
  | Seq of expr list
  | Set of expr * expr
  | Def of expr * expr
  | Or of expr list
  | LambdaSimple of string list * expr
  | LambdaOpt of string list * string * expr
  | Applic of expr * (expr list);;

type var =
  | VarFree' of string
  | VarParam' of string * int
  | VarBound' of string * int * int;;

type expr' =
  | Const' of sexpr
  | Var' of var
  | Box' of var
  | BoxGet' of var
  | BoxSet' of var * expr'
  | If' of expr' * expr' * expr'
  | Seq' of expr' list
  | Set' of expr' * expr'
  | Def' of expr' * expr'
  | Or' of expr' list
  | LambdaSimple' of string list * expr'
  | LambdaOpt' of string list * string * expr'
  | Applic' of expr' * (expr' list)
  | ApplicTP' of expr' * (expr' list);;
exception X_syntax_error;;

module type TAG_PARSER = sig
  val read_expression : string -> expr
  val read_expressions : string -> expr list
  val expression_to_string : expr -> string
end;; (* signature TAG_PARSER *)

module Tag_Parser : TAG_PARSER = struct

open PC;;

let reserved_word_list =
  ["and"; "begin"; "cond"; "define"; "do"; "else";
   "if"; "lambda"; "let"; "let*"; "letrec"; "or";
   "quasiquote"; "quote"; "set!"; "unquote";
   "unquote-splicing"];;

let is_reserved var =
  ormap (fun word -> var = word) reserved_word_list;;

let is_proper_list lst =
  let rec check lst =
    match lst with
    | Nil -> true
    | Pair (x, Nil) -> true
    | Pair (x, Pair(y, z)) -> check z
    | _ -> false
    in check lst;;

let rec break_optional_list params =
  let rec breaker lst =
    match lst with
    | Pair (x, Pair(y, z)) -> x :: y :: (breaker z)
    | Pair (x, y) -> [x]
    | _ -> []
    in breaker params;;

let get_optional_list params =
  let rec finder lst =
    match lst with
    | Pair (x, Pair(y, z)) -> finder z
    | Pair (Symbol(x), Symbol(y)) -> y
    | Symbol s -> s
    | _ -> raise X_this_should_not_happen
    in finder params;;

let rec process_scheme_list s ret_nil ret_one ret_several =
  match s with
  | Nil -> ret_nil ()
  | (Pair(sexpr, sexprs)) ->
     process_scheme_list sexprs
       (fun () -> ret_one sexpr)
       (fun sexpr' -> ret_several [sexpr; sexpr'])
       (fun sexprs -> ret_several (sexpr :: sexprs))
  | _ -> raise X_syntax_error;;

let scheme_list_to_ocaml_list args =
  process_scheme_list args
          (fun () -> [])
          (fun sexpr -> [sexpr])
          (fun sexprs -> sexprs);;

let expand_let_star ribs sexprs =
  let ribs = scheme_list_to_ocaml_list ribs in
  let params = List.map (function
        | (Pair(name, (Pair(expr, Nil)))) -> name
        | _ -> raise X_this_should_not_happen) ribs in
  let args = List.map
         (function
     | (Pair(name, (Pair(expr, Nil)))) -> expr
     | _ -> raise X_this_should_not_happen) ribs in
  let params_set = List.fold_right
         (fun a s ->
          if (ormap
          (fun b ->
           (match (a, b) with
            | (Symbol a, Symbol b) -> a = b
            | _ -> raise X_this_should_not_happen))
          s)
          then s else a :: s)
         params
         [] in
  let place_holders = List.fold_right
      (fun a s -> Pair(a, s))
      (List.map
         (fun var -> (Pair(var, (Pair((Bool false), Nil)))))
         params_set)
      Nil in
  let assignments = List.map2
          (fun var expr ->
           (Pair((Symbol("set!")),
           (Pair(var, (Pair(expr, Nil)))))))
          params args in
  let body = List.fold_right
         (fun a s -> Pair(a, s))
         assignments
         sexprs in
  (Pair((Symbol("let")), (Pair(place_holders, body))));;

let expand_letrec ribs sexprs =
  let ribs = scheme_list_to_ocaml_list ribs in
  let params = List.map (function
        | (Pair(name, (Pair(expr, Nil)))) -> name
        | _ -> raise X_this_should_not_happen) ribs in
  let args = List.map
         (function
     | (Pair(name, (Pair(expr, Nil)))) -> expr
     | _ -> raise X_this_should_not_happen) ribs in
  let ribs = List.map
         (function
     | (Pair(name, (Pair(expr, Nil)))) ->
        (Pair(name, (Pair(Bool false, Nil))))
     | _ -> raise X_this_should_not_happen)
         ribs in
  let body = List.fold_right
         (fun a s -> Pair(a, s))
         (List.map2
      (fun var expr ->
       (Pair((Symbol("set!")),
       (Pair(var, (Pair(expr, Nil)))))))
      params args)
         sexprs in
  let ribs = List.fold_right
         (fun a s -> Pair(a, s))
         ribs
         Nil in
  (Pair((Symbol("let")), (Pair(ribs, body))));;

exception X_unquote_splicing_here_makes_no_sense;;

let rec expand_qq sexpr = match sexpr with
  | (Pair((Symbol("unquote")), (Pair(sexpr, Nil)))) -> sexpr
  | (Pair((Symbol("unquote-splicing")), (Pair(sexpr, Nil)))) ->
     raise X_unquote_splicing_here_makes_no_sense
  | (Pair(a, b)) ->
     (match (a, b) with
      | ((Pair((Symbol("unquote-splicing")), (Pair(a, Nil)))), b) ->
   let b = expand_qq b in
   (Pair((Symbol("append")),
         (Pair(a, (Pair(b, Nil))))))
      | (a, (Pair((Symbol("unquote-splicing")), (Pair(b, Nil))))) ->
   let a = expand_qq a in
   (Pair((Symbol("cons")), (Pair(a, (Pair(b, Nil))))))
      | (a, b) ->
   let a = expand_qq a in
   let b = expand_qq b in
   (Pair((Symbol("cons")), (Pair(a, (Pair(b, Nil)))))))
  | (Vector(sexprs)) ->
     let s = expand_qq (List.fold_right (fun a b -> Pair(a, b)) sexprs Nil) in
     (Pair((Symbol("list->vector")), (Pair(s, Nil))))
  | Nil | Symbol _ -> (Pair((Symbol("quote")), (Pair(sexpr, Nil))))
  | expr -> expr;;

let expand_cond sexprs =
  let sexprs = scheme_list_to_ocaml_list sexprs in
  let rec expander sexprs =
  match sexprs with
  (* else clause *)
  | Pair (Symbol "else", Pair (action, Nil)) :: [] -> action
  (* else clause isn't the last clause *)
  | Pair (Symbol "else", Pair (action, Nil)) :: rest -> raise X_syntax_error
  (* else clause, more than action to perform *)
  | Pair (Symbol "else", Pair (action, more_actions)) :: [] -> Pair (Symbol "begin", Pair (action, more_actions))
  (* else clause isn't the last clause, more than action to perform  *)
  | Pair (Symbol "else", Pair (action, more_actions)) :: rest -> raise X_syntax_error
  (* normal clause *)
  |Pair (pred, Nil) :: rest ->
      Pair ((Symbol "if"), Pair (pred, Pair (Void, Pair (expander rest, Nil))))
  | Pair (pred, Pair (action, Nil)) :: rest ->
      Pair ((Symbol "if"), Pair (pred, Pair (action, Pair (expander rest, Nil))))
  (* last clause wan't an else clause *)
  | [] -> Void
  (* normal clause, more than one action to perform *)
  | Pair (pred, Pair (action, more_actions)) :: rest ->
      Pair ((Symbol "if"), Pair (pred, Pair (Pair (Symbol "begin", Pair (action, more_actions)), Pair (expander rest, Nil))))
  | _ -> raise X_this_should_not_happen
  in expander sexprs;;

let expand_and sexprs =
  let rec expander lst =
  match lst with
  | Nil -> lst
  | Pair (x, Nil) -> x
  | Pair (x, y) ->  Pair (Symbol "if", Pair (x, Pair (expander y, Pair (Bool false, Nil))))
  | _ -> raise X_this_should_not_happen
  in expander sexprs;;

let ocaml_list_to_scheme_list lst =
  let rec converter sexprs =
  match sexprs with
  | [] -> Nil
  | [sexpr] -> Pair (sexpr, Nil)
  | car :: cdr -> Pair (car, converter cdr)
in converter lst;;

let get_ribs_from_hashtable tbl =
  let table_as_list = Hashtbl.fold (fun k (v, o) acc -> (k, (v, o)) :: acc) tbl [] in
  let table_as_list = List.sort (fun (k1, (v1, o1)) (k2, (v2, o2)) ->  if o1 < o2 then
                                                                          -1
                                                                        else if o1 > o2 then
                                                                              1
                                                                            else
                                                                              0)
                                table_as_list in
  let ribs = List.fold_right (fun (k, (v, o)) rest -> Pair (Pair (k, Pair (v, Nil)), rest)) table_as_list Nil in
    ribs;;

let contains_illegal_nested_definitions lst =
  let rec check lst =
  match lst with
  | [] -> false
  | [Pair (Symbol "begin", sexprs)] -> check (scheme_list_to_ocaml_list sexprs)
  | Pair (Symbol "begin", sexprs) :: tail -> check (scheme_list_to_ocaml_list sexprs) || check tail
  | [Pair (Symbol "define", rest)] -> true
  | Pair (Symbol "define", rest) :: tail -> true
  | head :: tail -> check tail
  in
  check lst;;

(* Pair (Symbol "lambda", Pair (param, body) )*)
let clear_nested_definitions body =
  let definitions_hashtbl = Hashtbl.create 50 in
  let body_as_list = scheme_list_to_ocaml_list body in
  let rec clear_defines sexpr_list order =
    match sexpr_list with
    | [] -> []
    | [car] ->
      begin
        match car with
          | Pair (Symbol "begin", sexprs) ->
            clear_defines (scheme_list_to_ocaml_list sexprs) order
          (* Deal with the 'MIT-syntax for define' in case this is an MIT define.
             this is done simply by convertig the body and args to lambda before
             addig it to our hash tbl.*)
          | Pair (Symbol "define", Pair (Pair(var, args), body)) ->
            Hashtbl.add definitions_hashtbl var ((Pair (Symbol "lambda", Pair (args, body))), !order);
            order := !order + 1;
            [];
          | Pair (Symbol "define", Pair (Symbol var, Pair (value, Nil))) ->
            begin
              Hashtbl.add definitions_hashtbl (Symbol var) (value, !order);
              order := !order + 1;
              [];
            end
          | _ -> [car]
      end
    | car :: cdr ->
      begin
        match car with
          | Pair (Symbol "begin", sexprs) ->
            (*
            DO NOT use List.append - it will evaluate the tail first,
            which will cause erroneous results.
            *)
            let head = clear_defines (scheme_list_to_ocaml_list sexprs) order in
            let tail = clear_defines cdr order in
            if head != [] then
              raise X_syntax_error
            else
              tail
            (* Deal with the 'MIT-syntax for define' in case this is an MIT define.
             this is done simply by convertig the body and args to lambda before
             addig it to our hash tbl.*)
          | Pair (Symbol "define", Pair (Pair(var, args), body)) ->
            begin
              Hashtbl.add definitions_hashtbl var ((Pair (Symbol "lambda", Pair (args, body))), !order);
              order := !order + 1;
              clear_defines cdr order;
            end
          | Pair (Symbol "define", Pair (Symbol var, Pair (value, Nil))) ->
            begin
              Hashtbl.add definitions_hashtbl (Symbol var) (value, !order);
              order := !order + 1;
              clear_defines cdr order
            end
          | _ -> car :: cdr
      end
    in
  let cleared_body = clear_defines body_as_list (ref(0)) in
  if cleared_body = [] || contains_illegal_nested_definitions cleared_body then
    raise X_syntax_error
  else
    let ribs = get_ribs_from_hashtable definitions_hashtbl in
    (* if the hash table is empty, there were no nested define expressions *)
    if ribs = Nil then
        ocaml_list_to_scheme_list cleared_body
    else
      Pair (Pair (Symbol "letrec", Pair (ribs, ocaml_list_to_scheme_list cleared_body)), Nil);;

let tag_parse sexpr =
  let rec run sexpr =
  (* let tag_parser = wrap_fun make_tag_parse in *)
    match sexpr with
    | Void -> Const Void
    | Bool value -> Const (Bool value)
    | Nil -> Const Nil
    | Number num -> Const (Number num)
    | Char c -> Const (Char c)
    | String s -> Const (String s)
    | Symbol s -> if is_reserved s then
                    raise X_syntax_error
                  else
                    Var s
    (* vector *)
    | Vector entries -> Const sexpr
    (* If w/ else clause *)
    | Pair (Symbol "if", Pair (test, Pair (dit, Nil))) ->
        If (run test, run dit, Const Void)
    (* If w/o else clause *)
    | Pair (Symbol "if", Pair (test, Pair (dit, Pair (dif, Nil)))) ->
        If (run test, run dit, run dif)
    (* cond *)
    | Pair (Symbol "cond", Pair (Nil, Nil)) -> raise X_syntax_error
    | Pair (Symbol "cond", clauses) ->
        if clauses = Nil then
          raise X_syntax_error
        else
          run (expand_cond clauses)
    (* let *)
    | Pair (Symbol "let", Pair (ribs, Nil)) ->
        raise X_syntax_error
    | Pair (Symbol "let", Pair (ribs, body)) ->
        let ribs_as_list = scheme_list_to_ocaml_list ribs in
        let args = List.map (fun sexpr ->
                                match sexpr with
                                | (Pair(Symbol(var), Pair(binding, Nil))) -> var
                                | _ -> raise X_this_should_not_happen)
                            ribs_as_list in
        let params = List.map (fun sexpr ->
                                match sexpr with
                                | (Pair(var, Pair(binding, Nil))) -> run binding
                                | _ -> raise X_this_should_not_happen)
                              ribs_as_list in
        let body_as_list = scheme_list_to_ocaml_list body in
        let body = List.map run body_as_list in
        let body =
          if List.length body = 1 then
            List.hd body
          else
            Seq body in
        Applic (LambdaSimple (args, body), params)
    (* let* *)
    | Pair (Symbol "let*", Pair (ribs, Nil)) ->
        raise X_syntax_error
    | Pair (Symbol "let*", Pair (ribs, body)) ->
        let cleared_body = clear_nested_definitions body in
        run (expand_let_star ribs cleared_body)
    (* letrec *)
    | Pair (Symbol "letrec", Pair (ribs, Nil)) ->
        raise X_syntax_error
    | Pair (Symbol "letrec", Pair (ribs, body)) ->
        let cleared_body = clear_nested_definitions body in
        run (expand_letrec ribs cleared_body)
    (* lambda *)
    | Pair (Symbol "lambda", Pair (sym, Nil)) -> raise X_syntax_error
    | Pair (Symbol "lambda", Pair (params, body)) ->
        let body = clear_nested_definitions body in
        let body = List.map run (scheme_list_to_ocaml_list body) in
        let body =
          if List.length body = 1 then
            List.hd body
          else
            Seq body in
            begin
              match params with
              | (Symbol(s)) -> LambdaOpt([], s, body)
              | _ ->  if is_proper_list params then
                        let params = List.map
                                            (fun param -> match param with
                                                          | Symbol par -> par
                                                          | _ -> raise X_this_should_not_happen)
                                            (scheme_list_to_ocaml_list params) in
                        LambdaSimple (params, body)
                       else
                        let optional = get_optional_list params in
                        let params = break_optional_list params in
                        let params = List.map
                                          (fun param -> match param with
                                                        | Symbol par -> par
                                                        | _ -> raise X_this_should_not_happen)
                                          params in
                                          LambdaOpt (params, optional, body)
            end
    (* or *)
    | Pair (Symbol "or", sexprs) ->
        let sexprs_as_list = scheme_list_to_ocaml_list sexprs in
        begin
        match sexprs_as_list with
        | [] -> Const (Bool false)
        | [sexpr] -> run sexpr
        | _ -> Or (List.map run sexprs_as_list)
        end
    (* and *)
    | Pair (Symbol "and", sexprs) ->
        let sexprs_as_list = scheme_list_to_ocaml_list sexprs in
        begin
        match sexprs_as_list with
        | [] -> Const (Bool true)
        | [sexpr] -> run sexpr
        | _ -> run (expand_and sexprs)
        end
    (* define *)
    | Pair (Symbol "define", Pair (Symbol var, Pair (value, Nil))) ->
        if is_reserved var then
          raise X_this_should_not_happen
        else
          let value = run value in
        Def (Var var, value);
    (* MIT-define *)
    | Pair (Symbol "define", Pair (Pair(var, args), body)) ->
        run (Pair (Symbol "define", Pair (var, Pair (Pair (Symbol "lambda", Pair (args, body)), Nil))))
    (* quote *)
    | Pair (Symbol "quote", Pair (sexprs, Nil)) ->
        Const sexprs
    (* quasiquote*)
    | Pair (Symbol "quasiquote", Pair (sexprs, Nil)) ->
        run (expand_qq sexprs)
    | Pair (Symbol "unquote", Pair (sexprs, Nil)) ->
        raise X_syntax_error
    (* begin *)
    | Pair (Symbol "begin", sexprs) ->
        let lst = List.map run (scheme_list_to_ocaml_list sexprs) in
        begin
        match lst with
        | [] -> Const Void
        | [expr] -> expr
        | expr :: exprs -> Seq lst
        end
    (* set! *)
    | Pair (Symbol "set!", Pair (Symbol var, Pair (new_val, Nil))) ->
        let new_val = run new_val in
        Set (Var var, new_val)
    (* application *)
    | Pair (operator, rest) ->
        Applic (run operator, List.map run (scheme_list_to_ocaml_list rest))
    in
    run sexpr;;

let read_expression string = tag_parse (Parser.read_sexpr string);;

let read_expressions string = List.map tag_parse (Parser.read_sexprs string);;

let rec lambda_params_to_string prev lst =
  match lst with
  | sexpr :: [] -> prev ^ sexpr
  | sexpr :: sexprs -> lambda_params_to_string (prev ^ sexpr ^ " ") sexprs
  | [] -> prev

let expression_to_string expr =
  let rec transform expr =
    match expr with
    | Const (Pair (x,y)) -> "'" ^ Sexpr.sexpr_to_string (Pair (x, y))
    | Const (Symbol sym) -> "'" ^ sym
    | Const c -> Sexpr.sexpr_to_string c
    | Var s -> s
    (* if w/o else clause *)
    | If (test, dit, Const Void) -> "(if "
                                    ^ (transform test)
                                    ^ " "
                                    ^ (transform dit)
                                    ^ ")"
    (* if w/ else clause *)
    | If (test, dit, dif) -> "(if "
                              ^ (transform test)
                              ^ " "
                              ^ (transform dit)
                              ^ " "
                              ^ (transform dif)
                              ^ ")"
    | Seq exprs -> (List.fold_left (fun prev expr -> prev ^ " " ^ (transform expr))
                                    "(begin"
                                    exprs)
                    ^ ")"
    | Set (var, new_val) -> "(set! "
                            ^ (transform var)
                            ^ " "
                            ^ (transform new_val)
                            ^ ")"
    | Def (var, value) -> "(define "
                          ^ (transform var)
                          ^ " "
                          ^ (transform value)
                          ^ ")"
    | Or exprs -> (List.fold_left (fun prev expr -> prev ^ " " ^ (transform expr))
                                  "(or"
                                  exprs)
                  ^ ")"
    | LambdaSimple (params, body) ->  (lambda_params_to_string "(lambda (" params)
                                      ^ ") "
                                      ^ (transform body)
                                      ^ ")"
    (* lambda variadic *)
    | LambdaOpt ([], optional, body) -> "(lambda "
                                        ^ optional
                                        ^ " "
                                        ^ (transform body)
                                        ^ ")"
    (* lambda optional *)
    | LambdaOpt (params, optional, body) -> (lambda_params_to_string "(lambda (" params)
                                            ^ " . "
                                            ^ optional
                                            ^ ") "
                                            ^ (transform body)
                                            ^ ")"
    | Applic (operator, operands) -> (List.fold_left
                                            (fun prev expr -> prev ^ " " ^ (transform expr))
                                            ("(" ^ (transform operator))
                                            operands)
                                      ^ ")"
    in transform expr;;



end;; (* struct Tag_Parser *)

module type SEMANTICS = sig
  val run_semantics : expr -> expr'
  val annotate_lexical_addresses : expr -> expr'
  val annotate_tail_calls : expr' -> expr'
  val box_set : expr' -> expr'
end;;

module Semantics : SEMANTICS = struct

exception X_unbound_param of string;;
exception X_no_such_element;;
exception X_empty_list;;
exception X_why of string;;

let annotate_lexical_addresses e =
  let global_env = Hashtbl.create 40 in
  let rec make_annotate_lex_add exp scope =
    match exp with
    | Var v ->
      begin
        if (Hashtbl.mem scope v) then
            Var' (Hashtbl.find scope v)
        else
           (* raise (X_unound_param v) *)
          Var' (VarFree' v)
      end
    | LambdaSimple  (lambda_params,body)->
      begin
        (* Create new envrionment *)
        let expanded_env = Hashtbl.create 30 in
        (* Copy all bound and add one to thier minors,
           make params from prefiuse lambda apper as bound *)
        Hashtbl.iter
         (fun k v ->
            match v with
            | VarBound' (name, major, minor) ->
                Hashtbl.replace expanded_env name (VarBound' (name, major + 1, minor))
            | VarParam' (name, minor) ->
                Hashtbl.replace expanded_env name (VarBound' (name, 0, minor))
            (* Keep free free *)
            | _ -> Hashtbl.replace expanded_env k v )
         scope;

        (* Add params to environemnt *)
        List.iteri
              (fun minor name ->
                 Hashtbl.replace expanded_env name (VarParam' (name, minor)))
              lambda_params;
        (* Call annotate_lexical_addresses recursivly *)
        LambdaSimple' (lambda_params, (make_annotate_lex_add body expanded_env))
      end
    | LambdaOpt (params, optional, body)->
      begin
        (* Create new envrionment *)
        let expanded_env = Hashtbl.create 30 in
        (* Copy all bound and add one to thier minors,
           make params from prefiuse lambda apper as bound *)
        Hashtbl.iter
         (fun k v ->
            match v with
            | VarBound' (name, major, minor) ->
                Hashtbl.replace expanded_env name (VarBound' (name, major + 1, minor))
            | VarParam' (name, minor) ->
                Hashtbl.replace expanded_env name (VarBound' (name, 0, minor))
            (* Keep free free *)
            | _ -> Hashtbl.replace expanded_env k v )
         scope;

        (* Add params to environemnt *)
        List.iteri
              (fun minor name ->
                 Hashtbl.replace expanded_env name (VarParam' (name, minor)))
              params;

        (* Add optional parameter to environemnt *)
        Hashtbl.replace expanded_env optional (VarParam' (optional, List.length params));

        (* Call annotate_lexical_addresses recursivly *)
        LambdaOpt' (params, optional, (make_annotate_lex_add body expanded_env))
      end
    | Applic (expr, expr_l) ->
        let expr_l' = (List.map (fun exp -> (make_annotate_lex_add exp scope)) expr_l) in
        Applic' ((make_annotate_lex_add expr scope), expr_l')
    | If (test,dit,dif) ->
        let   test' =  (make_annotate_lex_add test scope) in
        let   dit'  =  (make_annotate_lex_add dit scope)  in
        let   dif'  =  (make_annotate_lex_add dif scope)  in
      If' (test', dit', dif')
    | Seq expr_l ->
        (Seq' (List.map (fun exp -> (make_annotate_lex_add exp scope)) expr_l))
    | Set (name, exp) ->
        Set' ((make_annotate_lex_add name scope),  (make_annotate_lex_add exp scope))
    | Def (Var var, value) ->
      begin
        (* Calculate the value before to avoid bugs
           when overriding a variable *)
        let value' =(make_annotate_lex_add value scope) in

        (Hashtbl.add global_env var (VarFree' var));

        Def' (Var' (VarFree' var), value')
      end
    | Def (name, value) ->
        Def' ((make_annotate_lex_add name scope), (make_annotate_lex_add value scope))
    | Or expr_l ->
        Or' (List.map (fun exp -> (make_annotate_lex_add exp scope)) expr_l)
    | Const c ->
        Const' c
  in
  make_annotate_lex_add e global_env;;

let annotate_tail_calls e =
  let rec run e in_tail =
  match e with
  | Const' c -> Const' c
  | Var' var -> Var' var
  | If' (test, dit, dif) -> If' (run test false, run dit in_tail, run dif in_tail)
  | Seq' exprs' ->  Seq' (List.fold_right (fun expr' prev -> if prev = [] then
                                                            run expr' in_tail :: []
                                                          else
                                                            run expr' false :: prev)
                                      exprs'
                                      [])
  | Set' (var, value) -> Set' (var, run value false)
  | Def' (var, value) -> Def' (var, run value false)
  | Or' exprs' ->  Or' (List.fold_right (fun expr' prev -> if prev = [] then
                                                            run expr' in_tail :: []
                                                          else
                                                            run expr' false :: prev)
                                      exprs'
                                      [])
  | LambdaSimple' (params, body) -> LambdaSimple' (params, run body true)
  | LambdaOpt' (params, optional, body) -> LambdaOpt' (params, optional, run body true)
  | Applic' (operator, operands) -> if in_tail then
                                      ApplicTP' (run operator false,
                                                List.map (fun operand -> run operand false)
                                                          operands)
                                    else
                                      Applic' (run operator false, List.map (fun operand -> run operand false)
                                                                            operands)
  | _ -> raise X_this_should_not_happen
  in
  run e false;;

let push stack e =
  let stack = e :: stack in
  stack;;

let pop stack =
  if stack = [] then
    raise X_empty_list
  else
    List.tl stack;;

let rec update_stack_elem stack name g' s' b' =
  match stack with
    | [] -> raise X_no_such_element
    | (n, g, s, b) :: tail -> if n = name then
                                (name, g', s', b') :: tail
                              else
                                (n, g, s, b) :: (update_stack_elem tail name g' s' b');;

let rec remove_stack_elem stack name =
  match stack with
    | [] -> raise X_no_such_element
    | (n, g, s, b) :: tail -> if n = name then
                                tail
                              else
                                (n, g, s, b) :: (remove_stack_elem tail name);;

let stack_find stack name =
  try
    (*
    n  = var name;
    g = get occurrence;
    s = set occurrence;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    b = bound occurrence
    *)
    let (n, g, s, b) = List.find (fun e ->  match e with
                                            | (n, g, s, b) -> n = name)
                                  stack in
    (g, s, b)
  with Not_found -> raise X_no_such_element

let position_in_params params param =
  let rec finder params param index =
    match params with
    | [] -> raise X_no_such_element
    | head :: tail ->   if head = param then
                          index
                        else
                          finder tail param (index + 1)
  in
  finder params param 0;;

let contains lst e =
  try
    List.find (fun elem -> elem = e) lst;
    true
  with
    Not_found -> false

let box_set e =
  let rec updater expr' boxed_params =
  match expr' with
  | Const' c -> Const' c
  | Var' var ->
    begin
      match var with
      | VarFree' name -> Var' (VarFree' name)
      | VarParam' (name, minor) ->  if contains boxed_params name then
                                      BoxGet' (VarParam' (name, minor))
                                    else
                                      Var' (VarParam' (name, minor))
      | VarBound' (name, major, minor) -> if contains boxed_params name then
                                            BoxGet' (VarBound' (name, major, minor))
                                          else
                                            Var' (VarBound' (name, major, minor))
    end
  | Box' v -> expr'
  | BoxGet' v -> expr'
  | BoxSet' (v, value) -> expr'
  | If' (test, dit, dif) -> If' (updater test boxed_params, updater dit boxed_params, updater dit boxed_params)
  | Seq' exprs' ->  Seq' (List.map (fun e -> updater e boxed_params) exprs')
  | Set' (var, value) ->
    begin
      match var with
      | Var' (VarFree' (name)) -> Set' (var, updater value boxed_params)
      | Var' (VarParam' (name, minor)) -> if contains boxed_params name then
                                            BoxSet' (VarParam' (name, minor), updater value boxed_params)
                                          else
                                            Set' (Var' (VarParam' (name, minor)), updater value boxed_params)
      | Var' (VarBound' (name, major, minor)) ->  if contains boxed_params name then
                                                    BoxSet' (VarBound' (name, major, minor), updater value boxed_params)
                                                  else
                                                    Set' (Var' (VarBound' (name, major, minor)), updater value boxed_params)
      | _ -> raise (X_why "updater, set'")
    end
  (* TODO I think this case is irrelevant. we only define to the global environemnt, so there's no way we need to update such type instance *)
  | Def' (var, value) -> Def' (var, updater value boxed_params)
  | Or' exprs' ->  Or' (List.map (fun e -> updater e boxed_params)
                                  exprs')
  | LambdaSimple' (params, body) ->
    begin
      let body = updater body boxed_params in
      LambdaSimple' (params, body)
    end
  | LambdaOpt' (params, optional, body) ->
    begin
      let body = updater body boxed_params in
      LambdaOpt' (params, optional, body)
    end
  | Applic' (operator, operands) -> Applic' (updater operator boxed_params, List.map (fun operand -> updater operand boxed_params)
                                                                            operands)
  | ApplicTP' (operator, operands) -> ApplicTP' (updater operator boxed_params, List.map (fun operand -> updater operand boxed_params)
                                                                            operands)
  (* | _ -> raise (X_why "updater, no match") *)
  in
  let rec run expr' stack =
  match expr' with
  | Const' c -> Const' c
  | Var' var ->
    begin
      match var with
      | VarFree' name -> Var' (VarFree' name)
      | VarParam' (name, minor) ->  let (g, s, b) = stack_find !stack name in
                                    stack := update_stack_elem !stack name true s b;
                                    Var' (VarParam' (name, minor))
      | VarBound' (name, major, minor) -> let (g, s, b) = stack_find !stack name in
                                          stack := update_stack_elem !stack name true s true;
                                          Var' (VarBound' (name, major, minor))
    end
  | If' (test, dit, dif) -> If' (run test stack, run dit stack, run dif stack)
  | Seq' exprs' ->  Seq' (List.map (fun e -> run e stack) exprs')
  | Set' (var, value) ->
    begin
      match var with
      | Var' (VarFree' (name)) -> Set' (var, run value stack)
      | Var' (VarParam' (name, minor)) ->
        begin
          let (g, s, b) = stack_find !stack name in
          stack := update_stack_elem !stack name g true b;
          Set' (var, run value stack)
        end
      | Var' (VarBound' (name, major, minor)) ->
        begin
          let (g, s, b) = stack_find !stack name in
          stack := update_stack_elem !stack name g true true;
          Set' (var, run value stack)
        end
      | _ -> raise (X_why "run, Set'")
    end
  | Def' (var, value) -> Def' (var, run value stack)
  | Or' exprs' ->  Or' (List.map (fun e -> run e stack)
                                  exprs')
  | LambdaSimple' (params, body) ->
    begin
      List.iter (fun param -> stack := push !stack (param, false, false, false)) params;
      let body = run body stack in
      let params_to_box = List.filter (fun param -> try
                                                      let (g, s, b) = stack_find !stack param in
                                                      if g && s && b then
                                                        true
                                                      else
                                                        false
                                                    with X_no_such_element -> false)
                                      params in
      if params_to_box = [] then
        begin
          List.iter (fun param -> stack := pop !stack) params;
          LambdaSimple' (params, body)
        end
      else
        begin
          let body = updater body params_to_box in
          List.iter (fun param -> stack := pop !stack) params;
          LambdaSimple' (params, Seq' ((List.map  (fun param -> let minor = position_in_params params param in
                                                                Set' (Var' (VarParam' (param, minor)), Box' (VarParam' (param, minor))))
                                                  params_to_box)
                                        @ [body]))
        end
    end
  | LambdaOpt' (params, optional, body) ->
    begin
      List.iter (fun param -> stack := push !stack (param, false, false, false)) params;
      stack := push !stack (optional, false, false, false);
      let body = run body stack in
      let params_to_box = List.filter (fun param -> try
                                                      let (g, s, b) = stack_find !stack param in
                                                      if g && s && b then
                                                        true
                                                      else
                                                        false
                                                    with X_no_such_element -> false)
                                      (params @ [optional]) in
      if params_to_box = [] then
        begin
          stack := pop !stack; (* pop the parameter for the optional parameters list *)
          List.iter (fun param -> stack := pop !stack) params; (* pop all regular function parameters *)
          LambdaOpt' (params, optional, body)
        end
      else
        begin
          let body = updater body params_to_box in
          stack := pop !stack;
          List.iter (fun param -> stack := pop !stack) params;
          LambdaOpt' (params, optional, Seq' ((List.map  (fun param -> let minor = position_in_params (params @ [optional]) param in
                                                                Set' (Var' (VarParam' (param, minor)), Box' (VarParam' (param, minor))))
                                                  params_to_box)
                                              @ [body]))
        end
    end
  | Applic' (operator, operands) -> Applic' (run operator stack, List.map (fun operand -> run operand stack)
                                                                            operands)
  | ApplicTP' (operator, operands) -> ApplicTP' (run operator stack, List.map (fun operand -> run operand stack)
                                                                            operands)
  | _ -> raise (X_why "run, no match")
  in
  let stack = ref ([]) in
  run e stack;;

let run_semantics expr =
  box_set
    (annotate_tail_calls
       (annotate_lexical_addresses expr));;

end;; (* struct Semantics *)


let test_parser string =
  let expr = Tag_Parser.read_expression string in
  let string' = (Tag_Parser.expression_to_string expr) in
  Printf.printf "%s\n" string';;

(* module type CODE_GEN = sig
  val code_gen : expr' -> string
  val compile_scheme_file : string -> string -> unit
end;; *)

(* module Code_Gen : CODE_GEN = struct *)
module Code_Gen = struct

exception X_why of string;;

let t_VOID = 937610;;
let t_NIL = 722689;;
let t_BOOL = 741553;;
let t_CHAR = 181048;;
let t_INTEGER = 945311;;
let t_FRACTION = 451794;;
let t_STRING = 799345;;
let t_SYMBOL = 368031;;
let t_PAIR = 885397;;
let t_VECTOR = 335728;;
let t_CLOSURE = 276405;;

let file_to_string input_file =
  let in_channel = open_in input_file in
  let rec run () =
    try
      let ch = input_char in_channel in ch :: (run ())
    with End_of_file ->
      ( close_in in_channel;
        [] )
  in list_to_string (run ());;

let string_to_file output_file out_string =
  let out_channel = open_out output_file in
  ( output_string out_channel out_string;
    close_out out_channel );;

let lookup c lst =
  let rec finder lst2 =
    match lst2 with
    | [] -> raise (X_why "kaki")
    | (c2, addr, vals) :: tail -> if c2 = c then
                                    addr
                                  else
                                    finder tail
  in finder lst;;

let make_make_label name =
  let counter = ref 0
  in
  fun () ->
  ( counter := !counter + 1;
    Printf.sprintf "%s_%d" name (!counter) )

let make_if_else_labels =
  let make_if_else = make_make_label "L_if_else" in
  let make_else = make_make_label "L_else" in
  let make_if_end = make_make_label "L_if_end" in
  fun () ->
  (make_if_else(), make_else(), make_if_end());;

let make_or_labels =
  let make_entrance = make_make_label "L_or" in
  let make_exit = make_make_label "L_or_end" in
  fun () ->
  (make_entrance(), make_exit());;

let make_lambda_simple_labels =
  let make_start_expansion = make_make_label "L_simple_env_expansion" in
  let make_env_expand = make_make_label "L_simple_env_expand" in
  let make_env_end = make_make_label "L_simple_env_expand_end" in
  let make_param_copy = make_make_label "L_simple_param_copy" in
  let make_param_end = make_make_label "L_simple_param_copy_end" in
  let make_entrance = make_make_label "L_lambda_simple" in
  let make_exit = make_make_label "L_lambda_simple_end" in
  fun () ->
  (make_start_expansion(), make_env_expand(), make_env_end(), make_param_copy(), make_param_end(), make_entrance(), make_exit());;

let make_lambda_opt_labels =
  let make_entrance = make_make_label "L_lambda_opt" in
  let make_exit = make_make_label "L_lambda_opt_end" in
  fun () ->
  (make_entrance(), make_exit());;

let make_application_labels =
  let make_entrance = make_make_label "L_applic" in
  let make_exit = make_make_label "L_lapplic_end" in
  fun () ->
  (make_entrance(), make_exit());;

let make_application_tp_labels =
  let make_entrance = make_make_label "L_applic_tp" in
  let make_exit = make_make_label "L_applic_tp_end" in
  fun () ->
  (make_entrance(), make_exit());;

let const_table = ref [];;

let code_gen e =
  let rec run expr' env_size =
    match expr' with
    | Const' c -> "\tMOV(R0, IMM(MEM_START + " ^ string_of_int (lookup c !const_table) ^ "))\n"
    | Var' v ->
      begin
        match v with
        | VarFree' var -> "varfree"
        | VarParam' (var, minor) -> "MOV(R0, FPARG(" ^ string_of_int minor ^ " + 2))\n"
        | VarBound' (var, major, minor) -> "\tMOV(R0, FPARG(0))\n
                                            \tMOV(R0, INDD(R0, " ^ string_of_int major ^ "))\n
                                            \tMOV(R0, INDD(R0, " ^ string_of_int minor ^ "))\n"
      end
    (* | BoxGet' var -> "boxget" *)
    (* | BoxSet' (var, new_val) -> "boxset" *)
    | If' (test, dit, dif) ->
      let (in_label, else_label, end_label) = make_if_else_labels () in
      in_label ^ ":\n" ^
      "\t" ^ (run test env_size) ^ "\n" ^
      "\tCMP(R0, IND(MEM_START + 3))\n" ^
      "\tJUMP_EQ(" ^ else_label ^ ")\n" ^
      "\t" ^ (run dit env_size) ^ "\n" ^
      "\tJUMP(" ^ end_label ^ ")\n" ^
      else_label ^ ": \n" ^
      "\t" ^ (run dif env_size) ^ "\n" ^
      end_label ^ ":\n\n"
    | Seq' exprs' ->
      List.fold_right (fun curr prev -> (run curr env_size) ^ "\n" ^ prev)
                      exprs'
                      ""
    (* | Def' (var, value) -> "define" *)
    | Or' exprs' ->
      let (entrance, exit) = make_or_labels () in
      entrance ^ ":\n" ^
      (List.fold_right (fun e prev ->
                          (run e env_size) ^
                          "\tCMP(R0, INDD(MEM_START, 3))\n" ^
                          "\tJUMP_NE(" ^ exit ^ ")\n" ^
                          prev
                      )
                      exprs'
                      (exit ^ ":\n"))
    | LambdaSimple' (params, body) ->
      let (start_expansion, env_expand, env_end, param_copy, param_end, entrance, exit) = make_lambda_simple_labels () in
      let text =
        start_expansion ^ ":\n" ^
        "\tPUSH(IMM(" ^ string_of_int (env_size + 1) ^ "))\n" ^   (* R1 = |env| + 1*)
        "\tCALL(MALLOC)\n" ^        (* allocate space for the expanded environment *)
        "\tMOV(R1, R0)\n" ^             (* get the address of the space for the expanded environment *)
        "\tDROP(1)\n" ^
        "\tMOV(R2, FPARG(0))\n" ^   (* get the old environment *)
        "\tMOV(R3, IMM(0))\n" ^     (* i = 0 *)
        "\tMOV(R4, IMM(1))\n" ^     (* j = 1 *)
        env_expand ^ ":\n" ^                    (* expand environment for the new closure *)
        "\tCMP(R3, " ^ string_of_int env_size ^ ")\n" ^               (* loop entry condition, i < |env| *)
        "\tJUMP_EQ(" ^ env_end ^ ")\n" ^
        "\tMOV(R5, INDD(R2, R3))\n" ^           (* temp = R2[i] *)
        "\tMOV(INDD(R1, R4), R5)\n" ^           (* R1[j] = temp *)
        "\tINCR(R3)\n" ^                        (* i++ *)
        "\tINCR(R4)\n" ^                        (* j++ *)
        "\tJUMP(" ^ env_expand ^ ")\n" ^
        env_end ^ ":\n" ^
        "\tPUSH(FPARG(1))\n" ^                  (* push(n) *)
        "\tCALL(MALLOC)\n" ^
        "\tMOV(R3, R0)\n" ^                         (* R3 = MALLOC(n), 'param_copy' *)
        "\tDROP(1)\n" ^
        "\tMOV(R4, IMM(2))\n" ^                 (* i = 2, in order to skip the env and n that are on the stack *)
        "\tMOV(R5, FPARG(1))\n" ^
        "\tADD(R5, IMM(2))\n" ^                        (* R5 = n + 2 *)
        param_copy ^ ":\n" ^                          (* copy params to the heap *)
        "\tCMP(R4, R5)\n" ^                           (* loop entry condition, i < n *)
        "\tJUMP_EQ(" ^ param_end ^ ")\n" ^
        "\tMOV(R6, FPARG(R4))\n" ^                    (* temp = param[i] *)
        "\tMOV(INDD(R3, R4), R6)\n" ^                 (* param_copy[i] = temp *)
        "\tINCR(R4)\n" ^                              (* i++ *)
        "\tJUMP(" ^ param_copy ^ ")\n" ^
        param_end ^ ":\n" ^
        "\tMOV(INDD(R1, 0), R3)\n" ^                (*new_env[0] = param_copy *)
        "\tPUSH(IMM(3))\n" ^
        "\tCALL(MALLOC)\n" ^
        "\tDROP(1)\n" ^                             (* NOT IN MAYER'S NOTES *)
        "\tMOV(INDD(R0, 0), IMM(T_CLOSURE))\n" ^    (* set the closure's type tag *)
        "\tMOV(INDD(R0, 1), R1)\n" ^                (* attach the expanded environment to the closure *)
        "\tMOV(INDD(R0, 2), LABEL(" ^ entrance ^ "))\n" ^ (* attach the body of the function to the closure *)
        "\tJUMP(" ^ exit ^ ")\n" ^                        (* closure is built and its address is returned in R0 *)
        entrance ^ ":\n" ^                                (* when the closure is applied, body execution starts here *)
        "\tPUSH(FP)\n" ^                                  (* save old FP *)
        "\tMOV(FP, SP)\n" ^                               (* set new FP *)
        "MOV(R15, FPARG(1))" ^
        "CMP(R15, IMM(" ^ string_of_int (List.length params) ^ "))" ^
        "JUMP_NE(EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS)" ^
        "\t" ^ (run body (env_size + 1))^                 (* execute the function's nody *)
        "\tPOP(FP)\n" ^                                   (* restore old FP *)
        "\tRETURN\n" ^                                    (* body's value is in R0, return to the calling stack frame *)
        exit ^ ":\n"
        in text
    (* | LambdaOpt' (params, optional, body) -> "lambda opt" *)
    | Applic' (operator, operands) ->
      let (entrance, exit) = make_application_labels () in
      entrance ^ ":\n" ^
      (List.fold_right (fun operand prev -> (run operand env_size) ^ "\tPUSH(R0)\n" ^ prev)
                      operands
                      "") ^
      "\tPUSH(IMM(" ^ string_of_int (List.length operands) ^ "))\n" ^
      (run operator env_size) ^
      "\tCMP(IND(R0), IMM(T_CLOSURE))\n" ^
      "\tJUMP_NE(EXCPETION_APPLYING_NON_PROCEDURE)\n" ^
      "\tPUSH(INDD(R0, 1))\n" ^
      "\tCALLA(INDD(R0, 2))\n" ^
      "\tPOP(R1)\n" ^
      "\tPOP(R1)\n" ^
      "\tDROP(R1)\n" ^
      exit ^ ":\n"
    (* | ApplicTP' (operator, operands) -> "applic tp" *)
    | _ -> raise (X_why "codegen")
    in
  run e 0;;

let purge_duplicates lst =
  let rec purge lst2 =
    match lst2 with
    | [] -> []
    | head :: tail ->
      head :: purge (List.filter (fun e -> e <> head) tail)
  in
  purge lst;;

let pick_consts e =
  let rec run expr' =
    match expr' with
    | Const' c -> [c]
    | BoxSet' (var, new_val) -> run new_val
    | If' (test, dit, dif) -> run test @ run dit @ run dif
    | Seq' exprs' -> List.flatten (List.map run exprs')
    | Set' (var, new_val) -> run new_val
    | Def' (var, value) -> run value
    | Or' exprs' -> List.flatten (List.map run exprs')
    | LambdaSimple' (params, body) -> run body
    | LambdaOpt' (params, optional, body) -> run body
    | Applic' (operator, operands) -> run operator @ (List.flatten (List.map run operands))
    | ApplicTP' (operator, operands) -> run operator @ (List.flatten (List.map run operands))
    | _ -> []
    in
  run e;;

let expand_consts c =
  let rec expander c2 =
  match c2 with
  | Pair (head, tail) -> expander head @ expander tail @ [(Pair (head, tail))]
  | Vector entries -> (List.flatten (List.map expander entries)) @ [Vector entries]
  | _ -> [c2]
  in
  expander c;;

let string_to_list_of_chars str =
  List.map (fun c -> Char.code c)
            (string_to_list str);;

let vector_to_int_list entries =
  List.map (fun entry -> lookup entry !const_table) entries

let make_const_table lst =
  let prefix = [Void; Nil; Bool false; Bool true] in
  let first_pass = List.map pick_consts lst in
  let first_pass = List.flatten (purge_duplicates first_pass) in
  let second_pass =  List.flatten (List.map expand_consts first_pass) in
  let second_pass = purge_duplicates (prefix @ second_pass) in
  let counter = ref(6) in
  List.iter (fun e ->
              match e with
              | Void -> const_table := [(e, 0, [t_VOID])]
              | Nil ->  const_table := !const_table @ [(e, 1, [t_NIL])]
              | Bool false -> const_table := !const_table @ [(e, 2, [t_BOOL; 0])]
              | Bool true ->  const_table := !const_table @ [(e, 4, [t_BOOL; 1])]
              | Number (Int i) -> let k = !counter in
                                  counter := k + 2;
                                  const_table := !const_table @ [(e, k, [t_INTEGER; i])]
              | Number (Fraction {numerator; denominator}) -> let k = !counter in
                                                              counter := k + 3;
                                                              const_table := !const_table @ [(e, k, [t_FRACTION; numerator; denominator])]
              | Char c -> let k = !counter in
                          counter := k + 2;
                          const_table := !const_table @ [(e, k, [t_CHAR; Char.code c])]
              | String s -> let k = !counter in
                            counter := k + 2 + (String.length s);
                            const_table := !const_table @ [(e, k, [t_STRING] @ string_to_list_of_chars s)]
              | Symbol s -> let k = !counter in
                             counter := k + 2 + (String.length s);
                             const_table := !const_table @ [(e, k, [t_SYMBOL] @ string_to_list_of_chars s)]
              | Pair (head, tail) -> let k = !counter in
                                      counter := k + 3;
                                      const_table := !const_table @ [(e, k, [t_PAIR; lookup head !const_table; lookup tail !const_table])]
              | Vector entries -> let k = !counter in
                                  counter := k + 2 + (List.length entries);
                                  const_table := !const_table @ [(e, k, [t_VECTOR] @ vector_to_int_list entries)]
              )
            second_pass;;

let length_of_const_table = ref 0;;

let measure_length_of_const_table () =
  length_of_const_table :=
                          List.fold_right (fun (v, addr, vals) acc -> List.length vals + acc)
                                          !const_table
                                          0;;

let prologue =
"
#include <stdio.h>
#include <stdlib.h>

/* change to 0 for no debug info to be printed: */
#define DO_SHOW 1
#define MEM_START 1

#include \"cisc.h\"

int main()
{

  START_MACHINE
  JUMP(CONTINUE)

  #include \"char.lib\"
  #include \"io.lib\"
  #include \"math.lib\"
  #include \"scheme.lib\"
  #include \"string.lib\"
  #include \"system.lib\"

EXCPETION_APPLYING_NON_PROCEDURE:
  SHOW(\"Trying to apply a non-procedure: \", R0)
  HALT

EXCEPTION_WRONG_NUMBER_OF_ARGUMENTS:
  SHOW(\"Applying procedure on wrong number of parameters: \", FPARG(1))
  HALT

CONTINUE:\n";;

let epilogue =
  "
  PUSH(R0)
  CALL(WRITE_SOB)
  PUSH(IMM('\\n'))
  CALL(PUTCHAR)
  DROP(2)
  STOP_MACHINE

  return 0;
}";;

let char_to_string c =
  Char.escaped (Char.chr c);;

let const_table_to_string () =
  List.fold_right (fun (c, addr, vals) prev ->
                      let curr =
                        match c with
                        | Void -> "\tMOV(ADDR(MEM_START), IMM(T_VOID))\n"
                        | Nil -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_NIL))\n"
                        | Bool b -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_BOOL))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 1) ^ "), IMM(" ^ string_of_int (List.nth vals 1) ^ "))\n"
                        | Char c -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_CHAR))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 1) ^ "), IMM('" ^ char_to_string (List.nth vals 1) ^ "'))\n"
                        | Number (Int i) -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_INTEGER))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 1) ^ "), IMM(" ^ string_of_int (List.nth vals 1) ^ "))\n"
                        | Number (Fraction f) -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_FRACTION))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 1) ^ "), IMM(" ^ string_of_int (List.nth vals 1) ^ "))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 2) ^ "), IMM(" ^ string_of_int (List.nth vals 2) ^ "))\n"
                        | String s ->
                          let s_length = String.length s in
                          let prefix = "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_STRING))\n\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + 1), IMM(" ^ string_of_int s_length ^ "))\n" in
                          let index = ref(-2) in
                          let suffix =
                            List.fold_right (fun c prev -> index := !index + 1; "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + " ^ string_of_int (s_length - !index) ^ "), IMM('" ^ char_to_string c ^ "'))\n" ^ prev)
                                            (string_to_list_of_chars s)
                                            ""
                          in prefix ^ suffix
                        | Symbol s ->
                          let s_length = String.length s in
                          let prefix = "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_SYMBOL))\n\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + 1), IMM(" ^ string_of_int s_length ^ "))\n" in
                          let index = ref(-2) in
                          let suffix =
                            List.fold_right (fun c prev -> index := !index + 1; "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + " ^ string_of_int (s_length - !index) ^ "), IMM('" ^ char_to_string c ^ "'))\n" ^ prev)
                                            (string_to_list_of_chars s)
                                            ""
                          in prefix ^ suffix
                        | Pair (head, tail) -> "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_PAIR))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 1) ^ "), IMM(MEM_START + " ^ string_of_int (List.nth vals 1) ^ "))\n\tMOV(ADDR(MEM_START + " ^ string_of_int (addr + 2) ^ "), IMM(MEM_START + " ^ string_of_int (List.nth vals 2) ^ "))\n"
                        | Vector entries ->
                          let vec_length = List.length entries in
                          let prefix = "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ "), IMM(T_VECTOR))\n\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + 1), IMM(" ^ string_of_int vec_length ^ "))\n" in
                          let index = ref(-2) in
                          let suffix =
                            List.fold_right (fun c prev -> index := !index + 1; "\tMOV(ADDR(MEM_START + " ^ string_of_int addr ^ " + " ^ string_of_int (vec_length - !index) ^ "), IMM(MEM_START + " ^ (string_of_int (lookup (List.nth entries (vec_length - 2 - !index)) !const_table)) ^ "))\n" ^ prev)
                                            entries
                                            ""
                          in prefix ^ suffix
                      in
                      curr ^ prev
                    )
                  !const_table
                  "\n";;

let compile_scheme_file scm_source_file asm_target_file =
  let file_as_string = file_to_string scm_source_file in
  let file_as_expr'_list = List.map Semantics.run_semantics
                                    (Tag_Parser.read_expressions file_as_string) in
  begin
    make_const_table file_as_expr'_list;
    measure_length_of_const_table ();
    let code_as_list = List.map code_gen file_as_expr'_list in
    let code = List.fold_right (fun code prev_code -> code ^ prev_code)
                                code_as_list
                                "" in
    string_to_file asm_target_file (prologue ^
                                    "\tPUSH(IMM(" ^ (string_of_int !length_of_const_table) ^ "))\n\tCALL(MALLOC)\n\tDROP(1)\n" ^
                                    const_table_to_string () ^
                                    code ^
                                    epilogue)
  end
end;;

(* #trace Code_Gen.compile_scheme_file;; *)
(* #trace Code_Gen.const_table_to_string;; *)
(* #trace Code_Gen.char_to_string;; *)
(* #trace Code_Gen.measure_length_of_const_table;; *)
(* #trace Code_Gen.make_const_table;; *)
(* #trace Code_Gen.vector_to_int_list;; *)
(* #trace Code_Gen.string_to_list_of_chars;; *)
(* #trace Code_Gen.expand_consts;; *)
(* #trace Code_Gen.pick_consts;; *)
(* #trace Code_Gen.purge_duplicates;; *)
(* #trace Code_Gen.code_gen;; *)
(* #trace Code_Gen.lookup;; *)
(* #trace Code_Gen.file_to_string;; *)
(* #trace Code_Gen.string_to_file;; *)
