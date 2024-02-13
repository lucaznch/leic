package xxl.core;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import xxl.core.exception.UnrecognizedEntryException;


class Parser {
    private Spreadsheet _spreadsheet;



    Parser() {
    }


    Parser(Spreadsheet spreadsheet) {
        _spreadsheet = spreadsheet;
    }


    /**
    * parses a specified content into one of its subcontents
    *
    * @param contentSpecification - specification in a string format of the content 
    * @return - Content of the string contentSpecification
    * @throws - UnrecognizedEntryException
    */
    Content parseContent(String contentSpecification) throws UnrecognizedEntryException {
        char c = contentSpecification.charAt(0);

        if (c == '=') { // Reference or Function
            return parseContentExpression(contentSpecification.substring(1));
        }
        else { // Literal
            return parseLiteral(contentSpecification);
        }
    }


    /*
    * parses a specified literal into a string or int (literal)
    *
    * @param literalExpression - specification in a string format of the literal
    * @return - Literal of the string contentSpecification
    * @throws - UnrecognizedEntryException
    */
    private Literal parseLiteral(String literalExpression) throws UnrecognizedEntryException {
        if (literalExpression.charAt(0) == '\'') {
            return new LiteralString(literalExpression.substring(1));
        }
        else {
            try {
                int number = Integer.parseInt(literalExpression);
                return new LiteralInteger(number);
            }
            catch (NumberFormatException e) {
                throw new UnrecognizedEntryException("Número inválido: " + literalExpression);
            }
        }
    }


    /*
    * parses a specified content expression into a reference or a function
    *
    * @param contentSpecification - specification in a string format of the content
    * @return - Function or Reference of the string contentSpecification
    * @throws - UnrecognizedEntryException
    */
    private Content parseContentExpression(String contentSpecification) throws UnrecognizedEntryException {
        if (contentSpecification.contains("(")) { /* Function */
            return parseFunction(contentSpecification);
        }
        else { /* Reference */
            String[] address = contentSpecification.split(";");
            int row = Integer.parseInt(address[0]);
            int column = Integer.parseInt(address[1]);

            if (_spreadsheet != null)
                return new Reference(row, column, _spreadsheet);
            else
                return new Reference(row, column);
        }
    }


    /*
    * parses a specified function into a binary or range function
    *
    * @param functionSpecification - specification in a string format of the function
    * @return - BinaryFunction or RangeFunction of the string contentSpecification
    * @throws - UnrecognizedEntryException
    */
    private Content parseFunction(String functionSpecification) throws UnrecognizedEntryException {
        String[] components = functionSpecification.split("[()]");
        String functionName = components[0];
        String args = components[1];

        if (args.contains(",")) { // BinaryFunction
            return parseBinaryFunction(functionName, args);
        }
        else { // RangeFunction
            return parseRangeFunction(functionName, args);
        }
    }


    /*
    * parses a specified binary function
    *
    * @param functionName - name of the binary function
    * @param args - specification in a string format of the function arguments
    * @return - BinaryFunction of the string contentSpecification
    * @throws - UnrecognizedEntryException
    */
    private Content parseBinaryFunction(String functionName, String args) throws UnrecognizedEntryException {
        String[] arguments = args.split(",");
        Content arg1 = parseArgumentExpression(arguments[0]);
        Content arg2 = parseArgumentExpression(arguments[1]);

        if (functionName.equals("ADD")) 
            return new Add(arg1, arg2);
        else if (functionName.equals("SUB"))
            return new Sub(arg1, arg2);
        else if (functionName.equals("MUL"))
            return new Mul(arg1, arg2);
        else if (functionName.equals("DIV"))
            return new Div(arg1, arg2);
        else {
            throw new UnrecognizedEntryException("Função inválida: " + functionName);
        }
    }


    /*
    * parses the arguments of a binary function
    *
    * @param argExpression - specifiation in a string format of the function arguments
    * @return - Reference or LiteralInteger of the string argExpression
    * @throws - UnrecognizedEntryException
    */
    private Content parseArgumentExpression(String argExpression) throws UnrecognizedEntryException {
        /* if the argument contains ';' => has a reference to a cell */
        if (argExpression.contains(";") && argExpression.charAt(0) != '\'') {
            String[] address = argExpression.split(";");
            int row = Integer.parseInt(address[0]);
            int column = Integer.parseInt(address[1]);

            if (_spreadsheet != null)
                return new Reference(row, column, _spreadsheet);
            else
                return new Reference(row, column);
        }
        else {
            return parseLiteral(argExpression);
        }
    }


    private Content parseRangeFunction(String functionName, String rangeDescription) throws UnrecognizedEntryException {
        Range range = _spreadsheet.createRange(rangeDescription);

        if (functionName.equals("AVERAGE")) {
            return new Average(range);
        }
        else if (functionName.equals("PRODUCT")) {
            return new Product(range);
        }
        else if (functionName.equals("CONCAT")) {
            return new Concat(range);
        }
        else if (functionName.equals("COALESCE")) {
            return new Coalesce(range);
        }
        else {
            throw new UnrecognizedEntryException("Função inválida: " + functionName);
        }
    }








    /*
    * parses the information from a file
    *
    * @param filename - name of the file
    * @return - spreadsheet
    * @throws - IOException, UnrecognizedEntryException
    */
    Spreadsheet parseFile(String filename) throws IOException, UnrecognizedEntryException {
        
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            parseDimensions(reader);

            String line;

            while ((line = reader.readLine()) != null) { /* EOF detection */
                parseLine(line);
            }
        }

        return _spreadsheet;
    }


    /*
    * parses the dimensions, from the file, to create the spreadsheet
    *
    * @param reader - stream
    * @throws - IOException, UnrecognizedEntryException
    */
    private void parseDimensions(BufferedReader reader) throws IOException, UnrecognizedEntryException {
        int rows = -1;
        int columns = -1;

        for (int i = 0; i < 2; i++) {
            String[] dimension = reader.readLine().split("=");

            if (dimension[0].equals("linhas"))
                rows = Integer.parseInt(dimension[1]);
            
            else if (dimension[0].equals("colunas"))
                columns = Integer.parseInt(dimension[1]);
            
            else
                throw new UnrecognizedEntryException("Linha inválida: " + reader.readLine());
        }

        if (rows <= 0 || columns <= 0)
            throw new UnrecognizedEntryException("Dimensões inválidas para a folha");

        _spreadsheet = new Spreadsheet(rows, columns);
        
    }


    /*
    * parses one line from the file
    *
    * @param line - line from the file
    * @throws - UnrecognizedEntryException
    */
    private void parseLine(String line) throws UnrecognizedEntryException {
        String[] components = line.split("\\|");

        if (components.length == 1) {
            /* there is no content in the cell given */
            return;
        }

        if (components.length == 2) {
            String[] address = components[0].split(";");
            Content content = parseContent(components[1]);
            
            int row = Integer.parseInt(address[0]);
            int column = Integer.parseInt(address[1]);
            _spreadsheet.insertContent(row, column, content);
        }
        else {
            throw new UnrecognizedEntryException("Wrong format in line: " + line);
        }
    }
}