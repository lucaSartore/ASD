fn main() {
    println!("{:?}",Solution::get_row(4));
}

struct Solution;

impl Solution {

    fn recursive(mut in_row: Vec<i32>, missing: i32) -> Vec<i32>{
        if missing == 0{
            return in_row
        }

        let mut prev = 0;
        for e in &mut in_row{
            let new_prev = *e;
            *e += prev;
            prev = new_prev
        }
        in_row.push(prev);

        Solution::recursive(in_row, missing-1)
    }

    pub fn get_row(row_index: i32) -> Vec<i32> {
        Self::recursive(vec![1],row_index)
    }
}