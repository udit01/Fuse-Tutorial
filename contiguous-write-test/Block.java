import java.math.BigInteger;
import java.util.*;

public class Block implements java.io.Serializable{

    public
    static int MAX_PF = 1000 ;
    static int MIN_PF = -1000;
    static int MAX_PARAM = 9;
    static int MIN_PARAM = 0;
    static int MAX_FACTORS = 100000;
    static int MIN_FACTORS = -100000;

    // Factors
    int hf ; // History Factor
    int uf ; // Usage Factor
    int sf ; // Spatial Factor
    int lf ; // Linking Factor
    int i; int j;

    // Used boolean
    boolean used ;

    Block(){
        this.hf = 0; // History Factor
        this.uf = 1; // Usage Factor
        this.sf = 0; // Spatial Factor
        this.lf = 1; // Linking Factor

        //we'll change mannually i, j after init
//        this.i = i_;
//        this.j = j_;
    }
}

class BlockComparator implements Comparator<Block>{
    public int compare(Block b1, Block b2){
            if(b1.j < b2.j){
                return 1;
            }
            else if(b1.j == b2.j){
                if(b1.i < b2.i){
                    return 1;
                }
                else if(b1.i == b2.i){
                    return 0;
                }
                else{
                    return -1;
                }
            }
            else{
                return -1;
            }
    }
}