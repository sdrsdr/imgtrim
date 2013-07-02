#!/usr/bin/php -f 
<?php
	$dw=55; $dh=54; $match_r=255; $match_g=255;$match_b=255; $match_tol=10;
	$im=imagecreatefromjpeg('184_2.jpg');
	$ch=imagesy($im); 
	$cw=imagesx($im); 
	if(!imageistruecolor($im)) {
		// Create a new true color image instance
		$tc = imagecreatetruecolor($cw, $ch);

		imagecopy($tc, $im, 0, 0, 0, 0, $cw, $ch);
		imagedestroy($im);

		$im = $tc;
		$tc = NULL;
	}
	
	$fw=round(($ch*$dw)/$dh);
	
	echo  ("target ar:%ar ".($dw/$dh)." current ar:".($cw/$ch)."(w:$cw/ h:$ch) width must be $fw to achieve ar ".($fw/$ch)." we need to remove ".($cw-$fw)."rows! \n");
	function testcol($c){
		global $im,$ch,$match_r,$match_g,$match_b, $match_tol;
		//echo "col$c:";
		for ($y=0;$y<$ch;$y++){
			$rgb= imagecolorat($im, $c, $y);
			$r = ($rgb >> 16) & 0xFF;
			$g = ($rgb >> 8) & 0xFF;
			$b = $rgb & 0xFF;
			//echo "$r,$g,$b|";
			if (abs($r-$match_r)>$match_tol || abs($g-$match_g)>$match_tol || abs($b-$match_b)>$match_tol) {
				//echo "FAIL!\n";
				return false;
			}
		}
		//echo "ok!\n";
		return true;
	}
	
	$leftat=0; $rightat=$cw-1; $canleft=true; $canright=true; $togo=($cw-$fw);
	while (($canleft || $canright) && $togo>0) {
		if ($canleft){
			if (testcol($leftat)){
				$leftat++;
				$togo--;
				if ($togo==0) break;
			} else $canleft=false;
		}
		if ($canright){
			if (testcol($rightat)){
				$rightat--;
				$togo--;
				if ($togo==0) break;
			} else $canright=false;
		}
	}
	if ($togo==0) {
		$dimg=imagecreatetruecolor($dw, $dh);
		imagecopyresampled($dimg,$im,0,0,$leftat,0,$dw,$dh,$fw,$ch);
		imagejpeg ( $dimg , "aspect-out.jpg");
		echo "DONE!\n";
	} else {
		echo "this cant be done atm!\n";
	}
	
?>