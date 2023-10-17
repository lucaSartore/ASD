use super::Solution;

#[test]
fn test(){
    let v1 = random_vector();
    let v2 = random_vector();
    let expected = safe_median(&v1, &v2);
    let got = Solution::find_median_sorted_arrays(v1,v2);
    println!("expected: {expected} got: {got}");
    assert_eq!(expected,got);
}

fn random(min: usize, max: usize) -> usize{
    rand::random::<usize>()%(max-min)+min
}

fn random_vector() -> Vec<i32>{
    let len = random(100,200);
    let mut vec = Vec::with_capacity(len);
    for _ in 0..len{
        vec.push(random(0,400) as i32);
    }
    vec.sort();
    return vec;
}

fn safe_median(v1: &[i32],v2: &[i32]) -> f64{
    let mut v1 = Vec::from(v1);
    let mut v2 = Vec::from(v2);

    v1.append(&mut v2);


    let mut total = v1;

    total.sort();

    if total.len()%2 == 1{
        total[total.len()/2] as f64
    }else {
        (total[total.len() / 2] as f64 + total[total.len() / 2 - 1] as f64) / 2.
    }
}

