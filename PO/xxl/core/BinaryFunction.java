package xxl.core;

public abstract class BinaryFunction extends Function {
    private Content _arg1, _arg2;



    public BinaryFunction() {
    }


    public BinaryFunction(String name) {
        super(name);
    }
    

    /** sets the arguments of a binary function (they can be a Reference or a LiteralInteger) */
    public void setArguments(Content arg1, Content arg2) {
        _arg1 = arg1;
        _arg2 = arg2;
    }


    /** 
    * @param whichArgument - specification of argument to return its value
    * (1 for arg1 and 2 for arg2)
    *
    * @return - value of the specified argument
    */
    public int getArg(int whichArgument) { 
        return (whichArgument == 1) ? _arg1.asInt() : _arg2.asInt();
    }


    /** @return - specification of Function to a string */
    @Override
    public String toString() {
        /* if arg1 and arg2 are both a reference, we remove the '=' */
        if (_arg1.toString().charAt(0) == '=' && _arg2.toString().charAt(0) == '=') {
            return "=" + getName() + "(" + _arg1.toString().substring(1) + "," + _arg2.toString().substring(1) + ")";
        }

        /** if arg1 is a reference, we remove the '=' */
        else if (_arg1.toString().charAt(0 )== '=') {
            return "=" + getName() + "(" + _arg1.toString().substring(1) + "," + _arg2.toString() + ")";
        }

        /** if arg2 is a reference, we remove the '=' */
        else if (_arg2.toString().charAt(0) == '=') {
            return "=" + getName() + "(" + _arg1.toString() + "," + _arg2.toString().substring(1) + ")";
        }

        else
            return "=" + this.getName() + "(" + _arg1.toString() + "," + _arg2.toString() + ")";
    }
}