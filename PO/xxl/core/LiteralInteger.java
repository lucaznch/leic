package xxl.core;

public class LiteralInteger extends Literal {
    private int _value;



    public LiteralInteger(int value) {
        _value = value;
    }


    /** 
    * @return - NumberFormatException 
    * @throws - NumberFormatException
    */
    public String asString() {
        throw new NumberFormatException("erro: " + _value + " não é string");
    }


    /** @return - value */
    public int asInt() {
        return _value;
    }


    /** @return - specification of LiteralInteger to a string */
    @Override
    public String toString() {
        return "" + _value;
    }


}