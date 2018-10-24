import java.nio.*;
import java.util.*;
import java.io.*;


public class Test{

	public static void main(String args[]) throws Exception{
		Block[][] a = new Block[20][20];
		PriorityQueue<Block> unusedBlocks;
		unusedBlocks = new PriorityQueue<>(400, new BlockComparator());

		for(int i = 0; i < 20; i++){
			for(int j = 0; j < 20; j++){
				a[i][j] = new Block();
				a[i][j].i = i;
				a[i][j].j = j;
			}
		}
		for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                unusedBlocks.add(a[i][j]);
            }
        }


		FileOutputStream fos = new FileOutputStream("test.ser");
		ObjectOutputStream out = new ObjectOutputStream(fos);
		out.writeObject(a);
		out.close();
		fos.close();
		


		Block[][] b = null;
		FileInputStream fis = new FileInputStream("test.ser");
		ObjectInputStream in = new ObjectInputStream(fis);
		b = (Block[][]) in.readObject();
		in.close();
		fis.close();
		
		for(int i = 0; i < 20; i++){
			for(int j = 0; j < 20; j++){
				System.out.println(b[i][j].i + b[i][j].j);
			}
		}
	}
}
