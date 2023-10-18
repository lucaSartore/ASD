
mod test;

fn main() {
    let v1 = vec![1,1,1];
    let v2 = vec![1,1,1];

    println!("{}",Solution::find_median_sorted_arrays(v1,v2));
}

pub struct Solution{}

impl Solution{

    pub fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
        Self::_median(&nums1, &nums2,0,0)
    }

    fn _median(mut v1: &[i32], mut v2: &[i32], mut count_left: usize, mut count_right: usize) -> f64{

        println!("{:?} {:?} {} {}",v1,v2,count_left,count_right);

        // handle one of the two been empty
        if v1.len() <= 2 || v2.len() <= 2{
            return Self::_finalize(v1,v2,count_left,count_right)
        }
        let v1_half = v1[v1.len()/2];
        let v2_half = v2[v2.len()/2];


        if count_left < count_right{
            // split to the left
            if v1_half < v2_half{
                // split v1
                let(sx,dx) = v1.split_at(v1.len()/2);
                v1 = dx;
                count_left += sx.len();
            }else{
                // split v2
                let (sx,dx) = v2.split_at(v2.len()/2);
                v2 = dx;
                count_left += sx.len();
            }
        }else{
            // split to the right
            if v1_half > v2_half{
                // split v1
                let (sx,dx) = v1.split_at(v1.len()/2+1);
                v1 = sx;
                count_right += dx.len();
            }else{
                // split v2
                let (sx,dx) = v2.split_at(v2.len()/2+1);
                v2 = sx;
                count_right += dx.len();
            }
        }

        Self::_median(v1,v2,count_left,count_right)
    }


    fn _finalize(v1: &[i32], v2: &[i32], mut count_left: usize, mut count_right: usize) -> f64{
        let mut long;
        let mut short;

        if v1.len() > v2.len(){
            long = v1;
            short = v2;
        }else{
            long = v2;
            short = v1;
        }

        let mut delta;
        if count_right > count_left{
            delta = count_right - count_left;
            delta = 0.max(delta as i32 - short.len() as i32) as usize;
            long = &long[delta..];
            count_left += delta;
        }else{
            delta = count_left - count_right;
            delta = 0.max(delta as i32 - short.len() as i32) as usize;
            long = &long[0..(long.len()-delta)];
            count_right += delta;
        }

        let mut total = Vec::from(long);
        total.append(&mut Vec::from(short));

        total.sort();

        let mut total_slice = total.as_slice();

        if count_right > count_left{
            delta = count_right-count_left;
            total_slice = &total_slice[delta..];
        }else{
            delta = count_left-count_right;
            total_slice = &total_slice[..(total_slice.len()-delta)];
        }

        if total_slice.len()%2 == 1{
            total_slice[total_slice.len()/2] as f64
        }else{
            (total_slice[total_slice.len()/2] as f64 + total_slice[total_slice.len()/2-1] as f64)/2.
        }
    }
}

