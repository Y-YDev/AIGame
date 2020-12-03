import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Scanner;

public class AI implements IPlayer {

    private boolean isP1 = false;
    public final static int VALMAX=96;
    public int maxDepth = 2;

    public AI(boolean isP1){
        this.isP1 = isP1;
    }


    @Override
    public int play(Board board){
        int maxResult= 0;
        int index = -1;
        for(int i = 0; i<board.cells.length/2;i++){
            Board playNext = new Board(board);
            playNext.playMove(convertIndex(i));

            int result = minMaxValue(playNext,1);
            if(index==-1){
                maxResult = result;
                index = i;
            }
            if(isP1 && result >maxResult){
                maxResult = result;
                index = i;
            }
            else if(!isP1 && result <maxResult ){
                maxResult = result;
                index = i;
            }
        }
        return convertIndex(index);//Case 1 d'index 0
    }

    public int minMaxValue(Board board,int depth){
        int sizeArray = board.cells.length/2;
        int[] scoreByIndex = new int[sizeArray];
        if(board.endPosition()){
            if (board.scoreP1 == board.scoreP2) return 0;
            else if (board.scoreP1 > board.scoreP2) return VALMAX;
            else return -VALMAX;
        }
        if(depth==maxDepth){
            return board.scoreP1-board.scoreP2;
        }
        for(int i = 0; i<sizeArray;i++){
            if(board.correctMove(convertIndex(i))){
                Board nextPos = new Board(board);
                nextPos.playMove(convertIndex(i));
                scoreByIndex[i] = minMaxValue(nextPos,depth+1);
            }
            else{
                if (board.P1Turn) scoreByIndex[i] = -10000;
                else scoreByIndex[i] = +10000;
            }
        }
        int res;
        if(isP1){
            // WRITE the code : res contains the MAX of tab_values
            int max = -1;
            for (int i = 0; i < 12; i++) {
                int score = scoreByIndex[i];
                if (max == -1 || max < score) {
                    max = score;
                }
            }
            res = max;
        }
        else {
            // WRITE the code: res contains the MIN of tab_values
            int min = -1;
            for (int i = 0; i < 12; i++) {
                int score = scoreByIndex[i];
                if (min == -1 || min > score) {
                    min = score;
                }
            }
            res = min;
        }
        return res;

    }

    private int convertIndex(int i){
        if(isP1){
            return i*2;
        }
        else{
            return i*2+1;
        }
    }



//    int minMaxValue(Board board, boolean P1Play, int depth) {
//        // computer_play is true if the computer has to play and false otherwise
//        int[] tab_values= new int[12];
//        Board pos_next ;// In C : created on the stack: = very fast
//
//        if (board.endPosition()) {
//            //WRITE the code: returns VALMAX (=96) if the computer wins, -96 if it loses; 0 if draw
//            if (board.scoreP1 == board.scoreP2) return 0;
//            else if (board.scoreP1 > board.scoreP2) return VALMAX;
//            else return -VALMAX;
//        }
//        if (depth == maxDepth) {
//            return evaluation(board, depth); //NEED TO WRITE IT ?
//            // the simplest evealution function is the difference of the taken seeds
//        }
//        for (int i = 0;i < 12;i++) {
//            // we play the move i
//            // WRITE function validMove(pos_current, computer_play,i)
//            // it checks whether we can select the seeds in cell i and play (if there is no seed the function returns false)
//            if (validMove(pos_current, computer_play, i)) {
//                // WRITE function playMove(&pos_next,pos_current, computer_play,i)
//                // we play the move i from pos_current and obtain the new position pos_next
//                playMove(&pos_next, pos_current, computer_play, i);
//                // pos_next is the new current position and we change the player
//                tab_values[i] = minMaxValue(&pos_next, !computer_play, depth + 1, depthMax);
//            }
//            else {//MAYBE FOR ELIMINATE IMPOSSIBLE MOVE
//                if (computer_play) tab_values[i] = -100;
//                else tab_values[i] = +100;
//            }
//        }
//        int res;
//        if (computer_play) {
//            // WRITE the code : res contains the MAX of tab_values
//            int max = -1;
//            for (int i = 0; i < 12; i++) {
//                int score = tab_values[i];
//                if (max == -1 || max < score) {
//                    max = score;
//                }
//            }
//            res = max;
//        }
//        else {
//            // WRITE the code: res contains the MIN of tab_values
//            int min = -1;
//            for (int i = 0; i < 12; i++) {
//                int score = tab_values[i];
//                if (min == -1 || min > score) {
//                    min = score;
//                }
//            }
//            res = min;
//        }
//        return res;
//    }
//
//
//    boolean validMove(Board pos_current, boolean computer_play, int i) {
//        int seed;
//        int idx;
//
//        if (computer_play) {
//            seed = pos_current->cells_computer[i];
//            if (seed == 0) return false;
//            idx = i + 11;
//        }
//        else {
//            seed = pos_current->cells_player[i];
//            if (seed == 0) return false;
//            idx = i;
//        }
//
//        if (needToBeFeed(pos_current, computer_play)) {//FEED RULE
//
//            for (int j = 0; j < seed; j++) {
//                idx = nextIndex(idx);
//
//                if (idx == i)//COMPLETE LOOP
//                    return true;
//            }
//
//            if (computer_play && idx > 11) {//DON'T FEED
//                return false;
//            }
//            if (!computer_play && idx < 12) {//DON'T FEED
//                return false;
//            }
//        }
//        return true;
//    }
//
//    boolean needToBeFeed(Board pos_current, boolean computer_play) {
//        boolean starve = true;
//
//        if (!computer_play) {//LOOK OPPONENT.
//            for (int i = 0;i < 12;i++) {
//                if (pos_current->cells_computer[i] != 0) starve = false;
//            }
//        }
//        else {
//            for (int i = 0;i < 12;i++) {
//                if (pos_current->cells_player[i] != 0) starve = false;
//            }
//        }
//
//        return starve;
//    }
//
//    int nextIndex(int i) {
//        if (i == 0) return 23;
//        else return i--;
//    }
//
//    void playMove(Board pos_next, Board pos_current, boolean computer_play, int i) {
//        int seed;
//        int idx;
//
//        copyPos(pos_next, pos_current);
//
//        if (computer_play) {
//            seed = pos_next->cells_computer[i];
//            idx = i + 11;
//        }
//        else {
//            seed = pos_next->cells_player[i];
//            idx = i;
//        }
//
//        while (seed != 0) {
//            idx = nextIndex(idx);
//            seed--;
//
//            if (idx > 11) {
//                pos_next->cells_computer[idx - 12]++;
//            }
//            else {
//                pos_next->cells_player[idx]++;
//            }
//        }
//
//        if (idx > 11 && !computer_play) {//COMPUTER ZONE AND PLAYER TURN
//            int nbSeed = pos_next->cells_computer[idx - 11];
//
//            while (idx < 24 && (nbSeed == 2 || nbSeed == 3)) {
//                pos_next->seeds_player += nbSeed;//TAKE
//                idx++;
//                pos_next->cells_computer[idx - 11] = 0;//REMOVE
//            }
//        }
//        else if(idx < 12 && computer_play){ //PLAYER ZONE AND COMPUTER TURN
//            int nbSeed = pos_next->cells_computer[idx];
//
//            while (idx < 12 && (nbSeed == 2 || nbSeed == 3)) {
//                pos_next->seeds_player += nbSeed;//TAKE
//                idx++;
//                pos_next->cells_computer[idx] = 0;//REMOVE
//            }
//        }
//    }
//
//
//    void copyPos(Board pos_next, Board pos_current) {
//
//    }
//
//

}
