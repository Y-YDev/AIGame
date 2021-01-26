package program;

import java.util.ArrayDeque;
import java.util.Deque;

public class Pool {
	Deque<Board> poolBoard;
	Deque<int[]> poolIntArray12;
	
	public Pool(int sizeBoard,int sizeIntArray) {
		allocPoolBoard(sizeBoard);
		allocPoolIntArray(sizeIntArray);
	}
	
	public void allocPoolBoard(int size) {
		poolBoard = new ArrayDeque<>(size);
		for(int i =0; i<size;i++) {
			Board current = new Board();
			current.allocBoard();
			poolBoard.push(current);
		}
	}
	
	public void allocPoolIntArray(int size) {
		poolIntArray12 = new ArrayDeque<>(size);
		for(int i =0; i<size;i++) {
			poolIntArray12.push(new int[12]);
		}
	}
	
	public Board pollBoard() {
		return poolBoard.poll();
	}
	
	public Board pollBoard(Board toCopy) {
		Board current = poolBoard.poll();
		current.copy(toCopy);
		return current;
	}
	
	public void pushBoard(Board board) {
		poolBoard.push(board);
	}
	
	
	
	public int[] pollIntArray12() {
		return poolIntArray12.poll();
	}
	
	public void pushIntArray12(int[] array) {
		poolIntArray12.push(array);
	}

}
