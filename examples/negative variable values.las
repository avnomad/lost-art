<?xml version="1.0" encoding="utf-8"?>
<!--
	Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>

	This file is part of LostArt.

	LostArt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LostArt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with LostArt.  If not, see <http://www.gnu.org/licenses/>.
-->

<!--
	Two controls of the same height and constant width are arranged
	next to each other so that they divide the empty space between
	and around them evenly in both directions.

	The variable 'a' representing the width of the empty space will
	take negative values if window width becomes too small to fit
	both controls.
-->
<gui-model>
	<controls>
		<control>
			<sides>
				<left>62.89063</left>
				<bottom>25.16945</bottom>
				<right>226.5974</right>
				<top>221.4694</top>
			</sides>
			<borderSize>2</borderSize>
			<name>Screen</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>89.56146</left>
				<bottom>209.1973</bottom>
				<right>127.6428</right>
				<top>139.7</top>
			</sides>
			<borderSize>2</borderSize>
			<name>bad</name>
			<nameHeight>10</nameHeight>
		</control>
		<control>
			<sides>
				<left>162.1979</left>
				<bottom>194.7334</bottom>
				<right>202.3948</right>
				<top>160.5139</top>
			</sides>
			<borderSize>2</borderSize>
			<name>ugly</name>
			<nameHeight>10</nameHeight>
		</control>
	</controls>
	<constraints>
		<constraint>
			<text>50mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>108.6833</first-local-side>
			<second-local-side>115.6833</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>175.7993</first-local-side>
			<second-local-side>182.7993</second-local-side>
		</constraint>
		<constraint>
			<text>300px</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>147.6653</first-local-side>
			<second-local-side>154.6653</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>right</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>right</side>
			</second-end-point>
			<first-local-side>175.3701</first-local-side>
			<second-local-side>182.3702</second-local-side>
		</constraint>
		<constraint>
			<text>a</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>1</control>
				<side>left</side>
			</first-end-point>
			<second-end-point>
				<control>0</control>
				<side>left</side>
			</second-end-point>
			<first-local-side>175.6347</first-local-side>
			<second-local-side>182.6347</second-local-side>
		</constraint>
		<constraint>
			<text>0mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>141.9492</first-local-side>
			<second-local-side>148.9492</second-local-side>
		</constraint>
		<constraint>
			<text>0mm</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>1</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>141.9492</first-local-side>
			<second-local-side>148.9492</second-local-side>
		</constraint>
		<constraint>
			<text>b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>top</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>bottom</side>
			</second-end-point>
			<first-local-side>208.327</first-local-side>
			<second-local-side>215.327</second-local-side>
		</constraint>
		<constraint>
			<text>b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>0</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>206.9165</first-local-side>
			<second-local-side>213.9165</second-local-side>
		</constraint>
		<constraint>
			<text>b</text>
			<text-height>7</text-height>
			<first-end-point>
				<control>2</control>
				<side>bottom</side>
			</first-end-point>
			<second-end-point>
				<control>2</control>
				<side>top</side>
			</second-end-point>
			<first-local-side>192.5479</first-local-side>
			<second-local-side>199.5479</second-local-side>
		</constraint>
	</constraints>
</gui-model>
