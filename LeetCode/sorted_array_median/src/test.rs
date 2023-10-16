use rand;

const MAX_LEN: usize = 50;
const VALUE_RANGE: i32 = 5_000;

#[test]
fn test(){
    use super::find_median_sorted_arrays;

    let(mut v1,v2) = get_random_arrays();

    if (v1.len() + v2.len())%2 == 0{
        let _ = v1.pop();
    }

    let merged = merge(&v1,&v2);

    println!("{:?}",v1);
    println!("{:?}",v2);
    println!("{:?}",merged);

    // test only odd
    let expected = merged[merged.len()/2] as f64;

    let got = find_median_sorted_arrays(v1,v2);

    println!("expected: {} got: {}",expected, got);

    assert_eq!(expected,got);

}


fn get_random_arrays() -> (Vec<i32>,Vec<i32>){
    fn gra() -> Vec<i32>{
        let len = 20;//rand::random::<usize>()%MAX_LEN + 100;
        let begin = 0;//rand::random::<i32>().abs()%VALUE_RANGE;
        let end = 50;//begin + rand::random::<i32>().abs()%VALUE_RANGE;

        let mut vec: Vec<_> = (0..len).into_iter().map(|_| rand::random::<i32>().abs()%(end-begin)+begin).collect();
        vec.sort();
        vec
    }
    (gra(),gra())
}

fn merge(left: &Vec<i32>, right: &Vec<i32>) -> Vec<i32> {
    let mut i = 0;
    let mut j = 0;
    let mut merged: Vec<i32> = Vec::new();

    while i < left.len() && j < right.len() {
        if left[i] < right[j] {
            merged.push(left[i]);
            i = i + 1;
        } else {
            merged.push(right[j]);
            j = j + 1;
        }
    }

    if i < left.len() {
        while i < left.len() {
            merged.push(left[i]);
            i = i + 1;
        }
    }

    if j < right.len() {
        while j < right.len() {
            merged.push(right[j]);
            j = j + 1;
        }
    }

    merged
}