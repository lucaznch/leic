package xxl.core;

public class LiteralString extends Literal {
    private String _value;



    public LiteralString(String value) {
        _value = value;
    }


    /** @return - value */
    public String asString() {
        return _value;
    }


    /**
    * @return - NumberFormatException
    * @throws - NumberFormatException
    */
    public int asInt() {
        throw new NumberFormatException("Erro: " + _value + " não é um inteiro");
    }


    /** @return - specification of LiteralString to a string */
    @Override
    public String toString() {
        return "\'" + _value;
    }

}