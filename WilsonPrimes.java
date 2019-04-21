import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;

/**
 * Created by Student on 4/3/2019.
 */

class WilsonPrimes implements Runnable {
    public static ArrayBlockingQueue<BigInteger> array = new ArrayBlockingQueue<>(1000);
    private Thread t;
    private String threadName;
    private BigInteger startInt;
    private BigInteger finish;
    private BigInteger increment;
    private ArrayList<ArrayList<BigInteger>> resultVal;
    private int value;

    WilsonPrimes(String name,String start, String finish, String increment, ArrayList<ArrayList<BigInteger>> result,int value) {
        threadName = name;
        this.value = value;
        this.resultVal = result;
        this.finish = new BigInteger(finish);
        this.startInt = new BigInteger(start);
        this.increment = new BigInteger(increment);
    }

    private boolean isPrime(BigInteger number){
        if(!number.isProbablePrime(5)) return false;

        for (BigInteger x = new BigInteger("2"); x.multiply(x).compareTo(number) < 0; x = x.add(BigInteger.ONE))
        {
            if(BigInteger.ZERO.equals(number.mod(x))) return false;
        }
        return true;
    }

    public boolean returnPrime(BigInteger number) {
        if (!number.isProbablePrime(20))
            return false;

        BigInteger twoBig = new BigInteger("2");
        if (BigInteger.ZERO.equals(number.mod(twoBig)))
            return false;

        for (BigInteger i = new BigInteger("3"); i.multiply(i).compareTo(number) < 1; i = i.add(twoBig)) {
            if (BigInteger.ZERO.equals(number.mod(i)))
                return false;
        }
        return true;
    }

    private BigInteger factorialIterative(BigInteger i){
        BigInteger fac = new BigInteger("1");
        for (BigInteger x = BigInteger.valueOf(2); x.compareTo(i) <= 0; x = x.add(BigInteger.ONE))
        {
            fac = fac.multiply(x);
        }
        return fac;
    }

    private boolean CheckWilson(BigInteger i){
        return BigInteger.ZERO.equals((factorialIterative(i.subtract(BigInteger.ONE)).add(BigInteger.ONE)).mod(i.multiply(i)));
    }

    public void run() {
        ArrayList<BigInteger> arrayList = new ArrayList<>();
        long start = System.currentTimeMillis();
        boolean checkIfThreadFoundWilson = false;
        for (BigInteger i = startInt; i.compareTo(finish) < 0; i = i.add(increment)) {
            if(returnPrime(i)){
                if(CheckWilson(i)){
                    checkIfThreadFoundWilson = true;
                    arrayList.add(i);
                }
            }
        }
        resultVal.add(arrayList);
        if(checkIfThreadFoundWilson){
            System.out.print("WILSON PRIME THREAD "+threadName+" ->");
            for(int i=0; i<arrayList.size();++i){
                System.out.print(" "+arrayList.get(i));
            }
            System.out.println();
        }
        long elapsedTime = System.currentTimeMillis() - start;
        System.out.println("EXIT Thread " +  threadName + " TIME "+ elapsedTime);
    }

    public void start () {
        if (t == null) {
            t = new Thread (this, threadName);
            t.start ();
        }
    }

    public void join() throws InterruptedException {
        if (t != null) {
            t.join();
        }
    }

    public static void main(String args[]) throws InterruptedException {
        WilsonPrimes wilsonPrimes[] = new WilsonPrimes[32];

        for(int threads=1; threads<=8; threads++){
            System.out.println("Start with "+threads+" threads");
            System.out.println("------------------------");
            ArrayList<ArrayList<BigInteger>> results = new ArrayList<>();
            int value = 0;
            for(int i=0; i<threads; ++i){
                wilsonPrimes[i] = new WilsonPrimes("Thread "+ i, String.valueOf(i+1),"35000",String.valueOf(threads), results,value);
                wilsonPrimes[i].start();
            }
            for(int i=0; i<threads; ++i){
                wilsonPrimes[i].join();
            }
            System.out.println("------------------------");
            System.out.println("Done with "+threads+" threads");
            System.out.print("\n\n");
        }
    }
}
